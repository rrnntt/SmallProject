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

  size_t n = vpot.n() + 1;
  std::cerr << "n=" << n << std::endl;
  //if (n < 3)
  {
    n = 200;
    vpot.resize( n );
  }

  const double beta = get("Beta");

  auto kinet = new ChebCompositeOperator;
  kinet->addRight( new ChebTimes(-beta) );
  kinet->addRight( new ChebDiff2 );
  auto hamiltonian = new ChebPlus;
  hamiltonian->add('+', kinet );
  hamiltonian->add('+', new ChebTimes(VPot) );

  GSLMatrix L;
  //size_t n = y.n() + 1;
  hamiltonian->createMatrix( vpot.getBase(), L );

  GSLVector d;
  GSLMatrix v;
  L.diag( d, v );

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

  //size_t imin = 0;
  //double dmin = d[0];
  //for(size_t i = 1; i < d.size(); ++i)
  //{
  //  const double tmp = d[i];
  //  if ( tmp < dmin )
  //  {
  //    dmin = tmp;
  //    imin = i;
  //  }
  //  std::cerr << i << ' ' << tmp << std::endl;
  //}

  //imin = 29;
  //y.setP( v, indx[0] );

  auto eigenvalues = API::TableWorkspace_ptr(dynamic_cast<API::TableWorkspace*>(
    API::WorkspaceFactory::instance().create("TableWorkspace"))
    );
  eigenvalues->setRowCount(n);
  setProperty("Eigenvalues", eigenvalues);

  eigenvalues->addColumn("double","N");
  auto nColumn = static_cast<API::TableColumn<double>*>(eigenvalues->getColumn("N").get());
  nColumn->asNumeric()->setPlotRole(API::NumericColumn::X);
  auto& nc = nColumn->data();

  eigenvalues->addDoubleColumn("Energy");
  auto& ec = eigenvalues->getDoubleData("Energy");

  auto eigenvectors = new ChebfunVector;
  for(size_t j = 0; j < n; ++j)
  {
    nc[j] = double(j);
    ec[j] = d[indx[j]];
    chebfun fun(n,startX,endX);
    fun.setP(v,indx[j]);
    eigenvectors->add(ChebFunction_sptr(new ChebFunction(fun)));
  }
  setProperty("Eigenvectors",ChebfunVector_sptr(eigenvectors));

  //for(size_t i = 0; i < n; ++i)
  //{
  //  std::string colName = "Y" + boost::lexical_cast<std::string>( i );
  //  tws->addColumn("double",colName);
  //  auto yColumn = static_cast<API::TableColumn<double>*>(tws->getColumn(colName).get());
  //  yColumn->asNumeric()->setPlotRole(API::NumericColumn::Y);
  //  auto& yc = yColumn->data();
  //  size_t k = indx[i];
  //  for(size_t j = 0; j < n; ++j)
  //  {
  //    yc[j] = v.get(j, k);
  //  }
  //}

  //setProperty("Table", tws);
}

} // namespace Numeric
