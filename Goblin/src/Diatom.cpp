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
  double calcKinet(size_t i, size_t j, FuncVector& ff, FuncVector& d2f, std::vector<double>& w, const double& beta);
  double calcPot(size_t i, size_t j, FuncVector& ff, std::vector<double>& w, const std::vector<double>& vpot);
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

  // equation now is -0.5*beta*d2/dr2 + ... where beta in cm-1 and r in angstroms

  std::cerr << "beta=" << beta << std::endl;

  // find the potential minimum

  ChebFunction v(2, rmin, rmax);
  v.fit( *Vpot );
  double r0 = 0;
  double alpha = 0;
  std::cerr << "n=" << v.cfun(0).n() << std::endl;
  {
    ChebFunction dv;
    dv.fromDerivative( v );
    FunctionDomain1DVector domain(rmin,rmax,100);
    FunctionValues values( domain );
    v.function( domain, values );

    auto vpotWS = API::TableWorkspace_ptr( new API::TableWorkspace );
    vpotWS->addColumn("double","x");
    vpotWS->getColumn("x")->asNumeric()->setPlotRole(API::NumericColumn::X);
    vpotWS->addColumn("double","vpot");
    vpotWS->getColumn("vpot")->asNumeric()->setPlotRole(API::NumericColumn::Y);
    vpotWS->setRowCount( domain.size() );

    auto& xcol = vpotWS->getDoubleData("x");
    auto& vcol = vpotWS->getDoubleData("vpot");
    for(size_t i = 0; i < domain.size(); ++i)
    {
      xcol[i] = domain[i];
      vcol[i] = values[i];
    }
    setProperty("VPotNum",vpotWS);

    std::vector<double> r;
    dv.roots( r );
    if ( r.size() != 1 )
    {
      throw std::runtime_error("Failed to find the potential minimum ("+boost::lexical_cast<std::string>(r.size())+")");
    }
    r0 = r[0];
    std::cerr << "Minimum at r0=" << r0 << std::endl;
    ChebFunction dv2;
    dv2.fromDerivative( dv );
    dv2.function1D(&alpha,&r0,1);
    std::cerr << "Alpha=" << alpha << std::endl;
    std::cerr << "E0=" << 0.5*sqrt(alpha*beta) << std::endl;
  }

  // compute the ground state 
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

  auto& r = quadWS->getDoubleData("r");
  auto& w = quadWS->getDoubleData("w");
  FuncVector ff(nmax);
  FuncVector d2f(nmax);
  for(size_t i = 0; i < ff.size(); ++i)
  {
    const std::string si = boost::lexical_cast<std::string>(i);
    ff[i] = &quadWS->getDoubleData("p"+si);
    d2f[i] = &quadWS->getDoubleData("dd"+si);
  }

  // sample the potential energy
  FunctionDomain1DView domain( r );
  FunctionValues values( domain );
  Vpot->function( domain, values );
  std::vector<double> vpot(nmax);
  for(size_t k = 0; k < nmax; ++k)
  {
    vpot[k] = values[k];
    std::cerr << k << ' ' << vpot[k] << std::endl;
  }

  std::ofstream fil("Matrix.txt");
  // build the hamiltonian matrix
  GSLMatrix H(nmax,nmax);
  for(size_t i =0; i < nmax; ++i)
  {
    for(size_t j =i; j < nmax; ++j)
    {
      double tmp = calcKinet(i,j,ff,d2f,w,beta);
      tmp += calcPot(i,j,ff,w,vpot);
      H.set(i,j, tmp);
      if ( i != j ) H.set(j,i, tmp);
      fil << std::setw(10) << tmp << ' ';
    }
    fil << std::endl;
  }
  fil.close();

  GSLVector ener;
  GSLMatrix ef;
  H.diag(ener,ef);

  std::cerr << ener << std::endl;
}

// privatenamespace
namespace
{
  /**
   * Calculate a matrix element of the kinetoc energy.
   * @param i :: The row index
   * @param j :: The column index
   * @param ff :: A vector of the basis functions
   * @param d2f :: A vector of the second derivatives of the basis functions
   * @param w :: The integration weights
   * @param beta :: The coefficient at the kinetic energy operator in the Schrodinger equation.
   * @return The value of the matrix element
   */
  double calcKinet(size_t i, size_t j, FuncVector& ff, FuncVector& d2f, std::vector<double>& w, const double& beta)
  {
    const size_t n = w.size();
    std::vector<double>& f = *ff[i];
    std::vector<double>& d2 = *ff[j];//*d2f[j];
    double res = 0.0;
    for(size_t k = 0; k < n; ++k)
    {
      res += f[k] * d2[k];//* w[k];
    }
    res *= -beta;
    return res;
  }

  double calcPot(size_t i, size_t j, FuncVector& ff, std::vector<double>& w, const std::vector<double>& vpot)
  {
    const size_t n = w.size();
    std::vector<double>& f1 = *ff[i];
    std::vector<double>& f2 = *ff[j];
    double res = 0;
    for(size_t k = 0; k < n; ++k)
    {
      res += f1[k] * f2[k] * vpot[k]; //* w[k];
    }
    return res;
  }
}

} // namespace Goblin
