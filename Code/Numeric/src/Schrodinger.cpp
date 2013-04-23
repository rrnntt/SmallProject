#include "Numeric/Schrodinger.h"
#include "Numeric/ChebfunWorkspace.h"
#include "Numeric/ChebOperator.h"

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

DECLARE_ALGORITHM(Schrodinger);

/// Constructor. Declare algorithm properties.
Schrodinger::Schrodinger()
{
  declareString("Operator");
  std::vector<std::string> boundary;
  //boundary.push_back("Dirichlet");
  //boundary.push_back("Cauchy");
  //declare("BoundaryConditions",new Kernel::StringProperty(boundary));
  declareString("BoundaryValues");
  declareDouble("StartX");
  declareDouble("EndX");
  declareInt("N");
  declareWorkspace("Chebfun");
  declareWorkspace("Table");
}

/// Execute algorithm.
void Schrodinger::exec()
{
  //API::WorkspaceProperty wsProp = get("Chebfun").as<API::WorkspaceProperty>();
  double startX = get("StartX");
  double endX = get("EndX");
  if (endX <= startX)
  {
    throw std::invalid_argument("StartX must be <= EndX");
  }
  int n = get("N");
  if (n < 2)
  {
    throw std::invalid_argument("N must be > 1");
  }
  std::string opStr = get("Operator");
  auto oper = ChebOperator::create( opStr );

  chebfun y;
  y.set(n,startX,endX);

  GSLMatrix L;
  //size_t n = y.n() + 1;
  oper->createMatrix( y.getBase(), L );

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
  y.setP( v, indx[0] );

  ChebfunWorkspace *cws = new ChebfunWorkspace( y );
  setProperty("Chebfun", boost::shared_ptr<ChebfunWorkspace>(cws));

  auto tws = API::TableWorkspace_ptr(dynamic_cast<API::TableWorkspace*>(
    API::WorkspaceFactory::instance().create("TableWorkspace"))
    );
  tws->setRowCount(n);

  tws->addColumn("double","X");
  auto xColumn = static_cast<API::TableColumn<double>*>(tws->getColumn("X").get());
  xColumn->asNumeric()->setPlotRole(API::NumericColumn::X);
  auto& xc = xColumn->data();

  tws->addDoubleColumn("Energy");
  auto& ec = tws->getDoubleData("Energy");
  for(size_t j = 0; j < n; ++j)
  {
    xc[j] = y.xpoints()[j];
    ec[j] = d[indx[j]];
  }

  for(size_t i = 0; i < n; ++i)
  {
    std::string colName = "Y" + boost::lexical_cast<std::string>( i );
    tws->addColumn("double",colName);
    auto yColumn = static_cast<API::TableColumn<double>*>(tws->getColumn(colName).get());
    yColumn->asNumeric()->setPlotRole(API::NumericColumn::Y);
    auto& yc = yColumn->data();
    size_t k = indx[i];
    for(size_t j = 0; j < n; ++j)
    {
      yc[j] = v.get(j, k);
    }
  }

  setProperty("Table", tws);
}

} // namespace Numeric
