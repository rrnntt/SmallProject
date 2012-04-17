#include "DataObjects/UniformChebfun.h"
#include "DataObjects/ChebfunWorkspace.h"
#include "DataObjects/TableWorkspace.h"
#include "DataObjects/TableColumn.h"

#include "API/AlgorithmFactory.h"
#include "API/WorkspaceProperty.h"

#include "Kernel/CommonProperties.h"

#include <algorithm>

namespace DataObjects
{

DECLARE_ALGORITHM(UniformChebfun);

UniformChebfun::UniformChebfun()
{
  declare("InputTable",new API::WorkspaceProperty(Kernel::Property::Input));
  declare("OutputChebfun",new API::WorkspaceProperty(Kernel::Property::Output));
  declare("XColumn",new Kernel::StringProperty);
  declare("YColumn",new Kernel::StringProperty);
}

void UniformChebfun::exec()
{
  API::WorkspaceProperty inProp = get("InputTable").as<API::WorkspaceProperty>();
  auto tws = inProp.to<TableWorkspace>();

  API::WorkspaceProperty outProp = get("OutputChebfun").as<API::WorkspaceProperty>();

  std::string xColumnName = get("XColumn");
  auto xColumn = static_cast<DataObjects::TableColumn<double>*>(tws->getColumn(xColumnName).get());
  auto& x = xColumn->data();

  std::string yColumnName = get("YColumn");
  auto yColumn = static_cast<DataObjects::TableColumn<double>*>(tws->getColumn(yColumnName).get());
  auto& y = yColumn->data();

  ChebfunWorkspace *cws = new ChebfunWorkspace;
  const size_t n = x.size();
  double start = x.front();
  double end = x.back();

  cws->fun().uniformFit(start,end,y);

  outProp = boost::shared_ptr<ChebfunWorkspace>(cws);

}

} // namespace DataObjects
