#include "Goblin/Diatom.h"

#include "Numeric/Constants.h"
#include "Numeric/IFunction.h"
#include "Numeric/FunctionFactory.h"
#include "Numeric/ChebFunction.h"
#include "Numeric/FunctionDomain1D.h"
#include "Numeric/FunctionValues.h"
#include "Numeric/GSLMatrix.h"
#include "Numeric/GSLVector.h"
#include "Numeric/AddFunctionValuesToTable.h"

#include "API/AlgorithmFactory.h"
#include "API/TableWorkspace.h"
#include "API/NumericColumn.h"

#include "Kernel/CommonProperties.h"

#include <boost/lexical_cast.hpp>
#include <stdexcept>
#include <fstream>

using namespace Numeric;

namespace Goblin
{

DECLARE_ALGORITHM(Diatom);

typedef std::vector< std::vector<double>* > FuncVector;

// private namespace forward declarations
namespace
{
  double calcKinet(size_t i, size_t j, FuncVector& d2f, std::vector<double>& w, const double& beta);
  double calcPot(size_t i, size_t j, FuncVector& ff, std::vector<double>& w, const std::vector<double>& vpot);
  struct CompEnergies
  {
    const GSLVector& m_vec;
    CompEnergies(const GSLVector& vec):m_vec(vec){}
    bool operator()(const size_t& i, const size_t& j)
    {
      return m_vec[i] < m_vec[j];
    }
  };
  void calcEigenFun(size_t i, std::vector<double>& fun, const FuncVector& ff, const GSLMatrix& ef, const std::vector<size_t>& indx);
}

/// Constructor. Declare algorithm properties.
Diatom::Diatom()
{
  declareDouble("mu"); // reduced mass in mass_p
  declareString("Vpot"); // in cm-1 as a function of r in angstroms
  declareInt("nmax",10);
  declareInt("Jmax",10);
  declareDouble("rmin",0.01);
  declareDouble("rmax",20.0); // the integration range [rmin,rmax] in angstrom
  declareWorkspace("Quad");
  declareWorkspace("Test");
  declareWorkspace("Test1");
  declareWorkspace("VPotNum");
}

/// Execute algorithm.
void Diatom::exec()
{
  double mu = get("mu");
  if ( mu <= 0 )
  {
    throw std::invalid_argument("mu cannot be negative or zero");
  }
  std::string funStr = get("Vpot");
  Numeric::IFunction_sptr Vpot = Numeric::FunctionFactory::instance().createFitFunction(funStr);
  int nmax = get("nmax");
  int Jmax = get("Jmax");
  const double rmin = get("rmin");
  const double rmax = get("rmax");

  std::cerr << planck_hbar * planck_hbar / 2 / mass_p << std::endl;
  std::cerr << planck_h * light_speed * 100 * angstrom * angstrom << std::endl;

  // convert hbar^2/mu -> beta in cm-1 and r in angstroms

  double beta = planck_hbar * planck_hbar / mass_p / mu / (planck_h * light_speed * 100 * angstrom * angstrom);
  //double beta = mu;

  // equation now is -0.5*beta*d2/dr2 + ... where beta in cm-1 and r in angstroms

  std::cerr << "beta=" << beta << std::endl;

  // the output workspace
  auto vpotWS = API::TableWorkspace_ptr( new API::TableWorkspace );
  vpotWS->addColumn("double","x");
  vpotWS->getColumn("x")->asNumeric()->setPlotRole(API::NumericColumn::X);
  setProperty("VPotNum",vpotWS);


  ChebFunction vpot(2, rmin, rmax);
  vpot.bestFit( *Vpot );
  std::cerr << "vpot n=" << vpot.cfun(0).n() << std::endl;

  // find the potential minimum r0 and harmonic constant alpha: 
  // vpot ~ 0.5*alpha*(x-r0)^2
  double r0 = 0;
  double alpha = 0;
  {
    // find the first derivative of vpot
    ChebFunction dv;
    dv.fromDerivative( vpot );
    // find the roots of dv. we expect only 1 root
    std::vector<double> r;
    dv.roots( r );
    if ( r.size() != 1 )
    {
      throw std::runtime_error("Failed to find the potential minimum ("+boost::lexical_cast<std::string>(r.size())+")");
    }
    // the found root is the potential minimum
    r0 = r[0];
    std::cerr << "Minimum at r0=" << r0 << std::endl;
    // find the second derivative of vpot
    ChebFunction dv2;
    dv2.fromDerivative( dv );
    // and calculate its value at r0: it is alpha
    dv2.function1D(&alpha,&r0,1);
    std::cerr << "Alpha=" << alpha << std::endl;
    std::cerr << "E0=" << 0.5*sqrt(alpha*beta) << std::endl;

  }

  // compute the harmonic quadrature
  {
    auto f0Fun = FunctionFactory::instance().createFitFunction( "UserFunction1D(Formula=exp(-a*(x-c)^2))" );
    f0Fun->setParameter("a",sqrt(alpha/beta));
    f0Fun->setParameter("c",r0);
    std::string intervalStr = boost::lexical_cast<std::string>(rmin)+","+boost::lexical_cast<std::string>(rmax);
    auto alg = API::AlgorithmFactory::instance().createAlgorithm("MakeQuadratureScheme");
    alg->setProperty("N",nmax);
    alg->setProperty("Interval",intervalStr);
    alg->setProperty("Function",f0Fun);
    alg->execute();

    API::TableWorkspace_ptr quadWS = alg->getClass("Quadrature");
    API::TableWorkspace_ptr testWS = alg->getClass("Test");

    setProperty("Quad",quadWS);
    setProperty("Test",testWS);
  }
  API::TableWorkspace_ptr quadWS = getClass("Quad");
  API::TableWorkspace_ptr testWS = getClass("Test");

  // quadrature abscissas
  auto& r = quadWS->getDoubleData("r");
  // quadrature weights
  auto& w = quadWS->getDoubleData("w");
  // basis function values at r[k] points (*sqrt(w[k]))
  FuncVector ff(nmax);
  // first derivatives of the basis functions (*sqrt(w[k]))
  FuncVector d1f(nmax);
  for(size_t i = 0; i < ff.size(); ++i)
  {
    const std::string si = boost::lexical_cast<std::string>(i);
    ff[i] = &quadWS->getDoubleData("p"+si);
    d1f[i] = &quadWS->getDoubleData("d"+si);
  }

  // sample the potential energy
  FunctionDomain1DView domain( r );
  {
    vpotWS->setRowCount( nmax );
    auto& xcol = vpotWS->getDoubleData("x");
    for(size_t k = 0; k < nmax; ++k)
    {
      xcol[k] = r[k];
    }
    AddFunctionValuesToTable::addColumn(vpotWS, "x", "vpot", vpot);
  }
  auto& v = vpotWS->getDoubleData("vpot");

  //std::ofstream fil("Matrix.txt");
  //std::ofstream filK("MatrixK.txt");
  //std::ofstream filP("MatrixP.txt");
  // build the hamiltonian matrix

  --nmax; // ?????
  GSLMatrix H(nmax,nmax);
  for(size_t i =0; i < nmax; ++i)
  {
    for(size_t j =i; j < nmax; ++j)
    {
      double tmpK = calcKinet(i,j,d1f,w,beta);
      //filK << std::setw(10) << tmpK << ' ';
      double tmpP = calcPot(i,j,ff,w,v);
      //filP << std::setw(10) << tmpP << ' ';
      H.set(i,j, tmpK + tmpP);
      if ( i != j ) H.set(j,i, tmpK + tmpP);
      //fil << std::setw(10) << tmpK + tmpP << ' ';
    }
    //fil << std::endl;
    //filK << std::endl;
    //filP << std::endl;
  }
  //fil.close();
  //filK.close();
  //filP.close();

  GSLVector ener;
  GSLMatrix ef;
  H.diag(ener,ef);

  std::cerr << ener << std::endl;
  std::vector<double> en(nmax);
  std::vector<size_t> indx(nmax);
  for(size_t i = 0; i < nmax; ++i) indx[i] = i;
  std::sort(indx.begin(), indx.end(),CompEnergies(ener) );

  for(size_t i = 0; i < nmax; ++i)
  {
    en[i] = ener[indx[i]];
  }
  
  for(size_t i = 0; i < nmax; ++i)
  {
    std::cerr << i << ' ' << en[i] ;
    if ( i > 0 ) std::cerr << ' ' << en[i] - en[i-1];
    std::cerr << std::endl;
  }

  // calculate the values of eigen-functions
  for(size_t i = 0; i < nmax; ++i)
  {
    const std::string colName = "fi" + boost::lexical_cast<std::string>(i);
    vpotWS->addColumn("double",colName);
    vpotWS->getColumn(colName)->asNumeric()->setPlotRole(API::NumericColumn::Y);

    auto& fi0 = vpotWS->getDoubleData(colName);
    calcEigenFun(i, fi0, ff, ef, indx);
  }

  // the output workspace with interpolated eigen-function values
  auto test1WS = API::TableWorkspace_ptr( new API::TableWorkspace );
  test1WS->addColumn("double","x");
  test1WS->getColumn("x")->asNumeric()->setPlotRole(API::NumericColumn::X);
  setProperty("Test1",test1WS);
}

//================================================================
// private namespace
//================================================================
namespace
{
  /**--------------------------------------------------------------------------------
   * Calculate a matrix element of the kinetic energy.
   * @param i :: The row index
   * @param j :: The column index
   * @param d1f :: A vector of the first derivatives of the basis functions
   * @param w :: The integration weights
   * @param beta :: The coefficient at the kinetic energy operator in the Schrodinger equation.
   * @return The value of the matrix element
   */
  double calcKinet(size_t i, size_t j, FuncVector& d1f, std::vector<double>& w, const double& beta)
  {
    const size_t n = w.size();
    std::vector<double>& d1 = *d1f[i];
    std::vector<double>& d2 = *d1f[j];
    double res = 0.0;
    for(size_t k = 0; k < n; ++k)
    {
      res += d1[k] * d2[k];
    }
    res *= beta/2;
    return res;
  }

  /**----------------------------------------------------------------------------
   * Calculate a matrix element of the potential energy.
   * @param i :: The row index
   * @param j :: The column index
   * @param ff :: A vector of the values of the basis functions
   * @param w :: The integration weights
   * @param vpot :: A vector of the values of the potential energy
   * @return The value of the matrix element
   */
  double calcPot(size_t i, size_t j, FuncVector& ff, std::vector<double>& w, const std::vector<double>& vpot)
  {
    const size_t n = w.size();
    std::vector<double>& f1 = *ff[i];
    std::vector<double>& f2 = *ff[j];
    double res = 0;
    for(size_t k = 0; k < n; ++k)
    {
      res += f1[k] * f2[k] * vpot[k];
    }
    return res;
  }

  /**--------------------------------------------------------------------------------
   * Calculate i-th eigenvector as a linear combination of basis functions.
   */
  void calcEigenFun(size_t i, std::vector<double>& fun, const FuncVector& ff, const GSLMatrix& ef, const std::vector<size_t>& indx)
  {
    std::cerr << "sizes: " << ff.size() << ' ' << ff[0]->size() << ' ' << indx.size() << ' ' << ef.size1() << std::endl;
    const size_t n = indx.size();
    fun.resize( n + 1 );
    size_t j = indx[i];
    for(size_t k = 0; k < n + 1; ++k)
    {
      double d = 0.0;
      for(size_t m = 0; m < n; ++m)
      {
        d += ef.get(m,j) * (*ff[m])[k];
      }
      fun[k] = d;
    }
  }

  /**-----------------------------------------------------------------------------
   * Calculate eigenfunctions as a linear combinations of basis functions.
   */
  void calcEigenFunction(TableWorkspace_sptr out, TableWorkspace_sptr quad, const GSLMatrix& ef, const std::vector<size_t>& indx)
  {
    const size_t n = indx.size();
    auto& c = quad->getDoubleData("c");
    auto& a = quad->getDoubleData("a");
    auto& b = quad->getDoubleData("b");

    for(size_t m = 0; m < n; ++m)
    {
      d += ef.get(m,j) * (*ff[m])[k];
    }
    fun[k] = d;
  }
}

} // namespace Goblin
