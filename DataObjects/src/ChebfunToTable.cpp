#include "DataObjects/ChebfunToTable.h"
#include "DataObjects/ChebfunWorkspace.h"
#include "DataObjects/TableWorkspace.h"
#include "DataObjects/TableColumn.h"

#include "Numeric/FunctionDomain1D.h"
#include "Numeric/FunctionValues.h"

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

  Numeric::FunctionDomain1D_sptr domain;

  size_t n = get("N").to<int>();

  if (n < 2)
  {// if n has default value (0) use the x-points of the chebfuns
    domain = cws->createDomainFromXPoints();
    n = domain->size();
  }
  else
  {// otherwise create a regular comb
    domain = cws->createDomain( n );
  }

  Numeric::FunctionValues values( *domain );
  cws->function(*domain, values);

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
  
  for(size_t i = 0; i < domain->size(); ++i)
  {
    x[i] = (*domain)[i];
    y[i] = values.getCalculated(i);
  }

  API::WorkspaceProperty outProp = get("OutputWorkspace").as<API::WorkspaceProperty>();
  outProp = tws;
}

} // DataObjects
