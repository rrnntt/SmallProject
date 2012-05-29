#include "Numeric/EvalChebfun.h"
#include "Numeric/ChebfunWorkspace.h"
#include "API/TableWorkspace.h"
#include "API/TableColumn.h"

#include "API/AlgorithmFactory.h"
#include "API/WorkspaceProperty.h"

#include "Kernel/CommonProperties.h"

namespace Numeric
{

DECLARE_ALGORITHM(EvalChebfun);

/// Constructor. Declare algorithm properties.
EvalChebfun::EvalChebfun()
{
  declare("Table",new API::WorkspaceProperty(Kernel::Property::InOut));
  declare("Chebfun",new API::WorkspaceProperty(Kernel::Property::Input));
  declare("XColumn",new Kernel::StringProperty);
  declare("YColumn",new Kernel::StringProperty);
}

/// Execute algorithm.
void EvalChebfun::exec()
{
  API::WorkspaceProperty tableProp = get("Table").as<API::WorkspaceProperty>();
  auto tws = tableProp.to<API::TableWorkspace>();

  API::WorkspaceProperty chebProp = get("Chebfun").as<API::WorkspaceProperty>();
  auto cws = chebProp.to<Numeric::ChebfunWorkspace>();

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

  for(size_t i = 0; i < x.size(); ++i)
  {
    y[i] = cws->fun(0)(x[i]);
  }
}

} // Numeric
