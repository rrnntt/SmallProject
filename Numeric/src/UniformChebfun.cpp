#include "Numeric/UniformChebfun.h"
#include "Numeric/ChebfunWorkspace.h"
#include "API/TableWorkspace.h"
#include "API/TableColumn.h"

#include "API/AlgorithmFactory.h"

#include "Kernel/CommonProperties.h"

#include <algorithm>

namespace Numeric
{

DECLARE_ALGORITHM(UniformChebfun);

UniformChebfun::UniformChebfun()
{
  declareClass("InputTable","WorkspaceManager");
  declareClass("OutputChebfun","WorkspaceManager");
  declareString("XColumn");
  declareString("YColumn");
}

void UniformChebfun::exec()
{
  API::TableWorkspace_ptr tws = getClass("InputTable");

  std::string xColumnName = get("XColumn");
  auto xColumn = static_cast<API::TableColumn<double>*>(tws->getColumn(xColumnName).get());
  auto& x = xColumn->data();

  std::string yColumnName = get("YColumn");
  auto yColumn = static_cast<API::TableColumn<double>*>(tws->getColumn(yColumnName).get());
  auto& y = yColumn->data();

  Numeric::ChebfunWorkspace *cws = new Numeric::ChebfunWorkspace;
  const size_t n = x.size();
  double start = x.front();
  double end = x.back();

  cws->fun(0).uniformFit(start,end,y);

  setProperty("OutputChebfun", boost::shared_ptr<Numeric::ChebfunWorkspace>(cws));

}

} // Numeric
