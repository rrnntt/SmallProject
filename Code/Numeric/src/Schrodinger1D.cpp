#include "Numeric/Schrodinger1D.h"
#include "Numeric/ChebfunWorkspace.h"
#include "Numeric/ChebOperator.h"
#include "Numeric/FunctionFactory.h"
#include "Numeric/ChebfunVector.h"

#include "API/AlgorithmFactory.h"
#include "API/TableWorkspace.h"
#include "API/TableColumn.h"
#include "API/NumericColumn.h"
#include "API/WorkspaceFactory.h"

#include "Kernel/CommonProperties.h"
#include "Kernel/EParser.h"

#include <boost/lexical_cast.hpp>

namespace Numeric
{

DECLARE_ALGORITHM(Schrodinger1D);

/// Constructor. Declare algorithm properties.
Schrodinger1D::Schrodinger1D()
{
  declareDouble("Beta");
  declareClass("VPot",&FunctionFactory::instance());
  declareDouble("StartX");
  declareDouble("EndX");
  declareWorkspace("Eigenvalues");
  declareWorkspace("Eigenvectors");
}

/// Execute algorithm.
void Schrodinger1D::exec()
{
  double startX = get("StartX");
  double endX = get("EndX");

  if (endX <= startX)
  {
    throw std::invalid_argument("StartX must be <= EndX");
  }

  IFunction_sptr VPot = getClass("VPot");
  chebfun vpot( 0, startX, endX );
  vpot.bestFit( *VPot );

  size_t nBasis = vpot.n() + 1;
  std::cerr << "n=" << nBasis << std::endl;
  //if (n < 3)
  {
    nBasis = 200;
    vpot.resize( nBasis );
  }

  const double beta = get("Beta");

  auto kinet = new ChebCompositeOperator;
  kinet->addRight( new ChebTimes(-beta) );
  kinet->addRight( new ChebDiff2 );
  auto hamiltonian = new ChebPlus;
  hamiltonian->add('+', kinet );
  hamiltonian->add('+', new ChebTimes(VPot) );

  GSLMatrix L;
  hamiltonian->createMatrix( vpot.getBase(), L );

  GSLVector d;
  GSLMatrix v;
  L.diag( d, v );

  std::vector<double> norms = vpot.baseNorm();
  assert(norms.size() == L.size1());
  assert(norms.size() == L.size2());

  for(size_t i = 0; i < norms.size(); ++i)
  {
      double factor = 1.0 / norms[i];
      for(size_t j = i; j < norms.size(); ++j)
      {
          v.multiplyBy(i,j,factor);
      }
  }

//  eigenvectors orthogonality check
//  GSLMatrix v1 = v;
//  GSLMatrix tst;
//  tst = Tr(v1) * v;
//  std::cerr << tst << std::endl;

  std::vector<size_t> indx(L.size1());
  {
    size_t i = 0;
    std::generate(indx.begin(), indx.end(), [&i]()->size_t{++i;return i-1;});
  }

  std::sort(indx.begin(), indx.end(), [&d](size_t x1,size_t x2)->bool{
    const double& d1 = d[x1];
    const double& d2 = d[x2];
    if ( d1 < 0 && d2 >= 0 ) return false;
    if ( d2 < 0 && d1 >= 0 ) return true;
    if ( d1 < 0 && d2 < 0 ) return d2 < d1;
    return d1 < d2;
  });

  auto eigenvalues = API::TableWorkspace_ptr(dynamic_cast<API::TableWorkspace*>(
    API::WorkspaceFactory::instance().create("TableWorkspace"))
    );
  eigenvalues->setRowCount(nBasis);
  setProperty("Eigenvalues", eigenvalues);

  eigenvalues->addColumn("double","N");
  auto nColumn = static_cast<API::TableColumn<double>*>(eigenvalues->getColumn("N").get());
  nColumn->asNumeric()->setPlotRole(API::NumericColumn::X);
  auto& nc = nColumn->data();

  eigenvalues->addDoubleColumn("Energy");
  auto eColumn = static_cast<API::TableColumn<double>*>(eigenvalues->getColumn("Energy").get());
  eColumn->asNumeric()->setPlotRole(API::NumericColumn::Y);
  auto& ec = eigenvalues->getDoubleData("Energy");

  auto eigenvectors = new ChebfunVector;
  chebfun fun0(nBasis,startX,endX);
  // collect indices of spurious eigenvalues to move them to the back
  std::vector<size_t> spurious;
  // index for good eigenvalues
  size_t n = 0;
  for(size_t j = 0; j < nBasis; ++j)
  {
    size_t i = indx[j];
    chebfun fun(fun0);
    fun.setP(v,i);

    // check eigenvalues for spurious ones
    chebfun dfun(fun);
    dfun.square();
    double norm = dfun.integr();

    // I am not sure that it's a solid condition
    if ( norm < 0.999999 )
    {
        // bad eigenvalue
        spurious.push_back(j);
    }
    else
    {
        nc[n] = double(n);
        ec[n] = d[i];
        eigenvectors->add(ChebFunction_sptr(new ChebFunction(fun)));
//        chebfun dfun(fun);
//        hamiltonian->apply(fun,dfun);
//        dfun *= fun;
//        std::cerr << "ener["<<n<<"]=" << ec[n] << ' ' << norm << ' ' << dfun.integr() << std::endl;
        ++n;
    }
  }

  // add spurious eigenvalues/vectors at the back
  for(size_t j = 0; j < spurious.size(); ++j,++n)
  {
      size_t i = indx[j];
      nc[n] = double(n);
      ec[n] = d[i];
      chebfun fun(fun0);
      fun.setP(v,i);
      eigenvectors->add(ChebFunction_sptr(new ChebFunction(fun)));
  }
  setProperty("Eigenvectors",ChebfunVector_sptr(eigenvectors));

}

} // namespace Numeric
