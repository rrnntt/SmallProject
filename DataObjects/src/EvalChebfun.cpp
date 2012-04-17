#include "DataObjects/EvalChebfun.h"
#include "DataObjects/ChebfunWorkspace.h"
#include "DataObjects/TableWorkspace.h"
#include "DataObjects/TableColumn.h"

#include "API/AlgorithmFactory.h"
#include "API/WorkspaceProperty.h"

#include "Kernel/CommonProperties.h"

namespace DataObjects
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
  auto tws = tableProp.to<TableWorkspace>();

  API::WorkspaceProperty chebProp = get("Chebfun").as<API::WorkspaceProperty>();
  auto cws = chebProp.to<ChebfunWorkspace>();

  std::string xColumnName = get("XColumn");
  auto xColumn = static_cast<DataObjects::TableColumn<double>*>(tws->getColumn(xColumnName).get());
  assert(xColumn);
  auto& x = xColumn->data();

  std::string yColumnName = get("YColumn");
  if ( !tws->hasColumn(yColumnName) )
  {
    tws->addColumn("double",yColumnName);
  }
  auto yColumn = static_cast<DataObjects::TableColumn<double>*>(tws->getColumn(yColumnName).get());
  assert(yColumn);
  auto& y = yColumn->data();
  yColumn->asNumeric()->setPlotRole(DataObjects::NumericColumn::Y);

  for(size_t i = 0; i < x.size(); ++i)
  {
    y[i] = cws->fun()(x[i]);
  }
}

} // namespace DataObjects
