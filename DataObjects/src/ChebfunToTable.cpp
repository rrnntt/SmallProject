#include "DataObjects/ChebfunToTable.h"
#include "DataObjects/ChebfunWorkspace.h"
#include "DataObjects/TableWorkspace.h"
#include "DataObjects/TableColumn.h"

#include "Numeric/FunctionDomain1D.h"

#include "API/AlgorithmFactory.h"
#include "API/WorkspaceProperty.h"
#include "API/WorkspaceFactory.h"

#include "Kernel/CommonProperties.h"

namespace DataObjects
{

  DECLARE_ALGORITHM(ChebfunToTable);

/**
 * Constructor.
 */
ChebfunToTable::ChebfunToTable()
{
  declare("InputWorkspace",new API::WorkspaceProperty(Kernel::Property::Input));
  declare("OutputWorkspace",new API::WorkspaceProperty(Kernel::Property::Output));
  declare("N",new Kernel::IntProperty);
}

void ChebfunToTable::exec()
{
  API::WorkspaceProperty wsProp = get("InputWorkspace").as<API::WorkspaceProperty>();
  std::string wsName = static_cast<std::string>(wsProp);
  auto cws = wsProp.to<ChebfunWorkspace>();
  if (!cws)
  {
    throw std::runtime_error("InputWorkspace property is not a ChebfunWorkspace");
  }
  int n = get("N").to<int>();
  if (n < 2)
  {
    n = cws->fun().n();
  }

  auto tws = DataObjects::TableWorkspace_ptr(dynamic_cast<DataObjects::TableWorkspace*>(
    API::WorkspaceFactory::instance().create("TableWorkspace"))
    );

  tws->addColumn("double","X");
  tws->addColumn("double","Y");
  tws->setRowCount(n);
  auto xColumn = static_cast<DataObjects::TableColumn<double>*>(tws->getColumn("X").get());
  xColumn->asNumeric()->setPlotRole(DataObjects::NumericColumn::X);
  auto& x = xColumn->data();
  auto yColumn = static_cast<DataObjects::TableColumn<double>*>(tws->getColumn("Y").get());
  yColumn->asNumeric()->setPlotRole(DataObjects::NumericColumn::Y);
  auto& y = yColumn->data();
  
  Numeric::chebfun& fun = cws->fun();
  if (n == fun.n())
  {
    x.assign(fun.xpoints().rbegin(),fun.xpoints().rend());
    y.assign(fun.ypoints().rbegin(),fun.ypoints().rend());
  }
  else
  {
    Numeric::FunctionDomain1DVector domain(fun.startX(),fun.endX(),n);
    for(size_t i = 0; i < domain.size(); ++i)
    {
      x[i] = domain[i];
      y[i] = fun.valueB(x[i]);
    }
  }

  API::WorkspaceProperty outProp = get("OutputWorkspace").as<API::WorkspaceProperty>();
  outProp = tws;
}

} // DataObjects
