#include "Numeric/EvalChebfun.h"
#include "Numeric/ChebfunWorkspace.h"
#include "API/TableWorkspace.h"
#include "API/TableColumn.h"

#include "API/AlgorithmFactory.h"

#include "Kernel/CommonProperties.h"

namespace Numeric
{

DECLARE_ALGORITHM(EvalChebfun);

/// Constructor. Declare algorithm properties.
EvalChebfun::EvalChebfun()
{
  declareWorkspace("Table");
  declareWorkspace("Chebfun");
  declareString("XColumn");
  declareString("YColumn");
}

/// Execute algorithm.
void EvalChebfun::exec()
{
  API::TableWorkspace_ptr tws = getClass("Table");

  ChebfunWorkspace_sptr cws = getClass("Chebfun");

  std::string xColumnName = get("XColumn");
  auto xColumn = static_cast<API::TableColumn<double>*>(tws->getColumn(xColumnName).get());
  assert(xColumn);
  auto& x = xColumn->data();

  std::string yColumnName = get("YColumn");
  if ( !tws->hasColumn(yColumnName) )
  {
    tws->addColumn("double",yColumnName);
  }
  auto yColumn = static_cast<API::TableColumn<double>*>(tws->getColumn(yColumnName).get());
  assert(yColumn);
  auto& y = yColumn->data();
  yColumn->asNumeric()->setPlotRole(API::NumericColumn::Y);

  FunctionDomain1DView domain(x.data(),x.size());
  FunctionValues values( domain );
  cws->eval(domain,values);

  for(size_t i = 0; i < x.size(); ++i)
  {
    y[i] = values.getCalculated( i );
  }
}

} // Numeric
