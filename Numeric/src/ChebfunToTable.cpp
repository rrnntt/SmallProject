#include "Numeric/ChebfunToTable.h"
#include "Numeric/ChebfunWorkspace.h"

#include "API/TableWorkspace.h"
#include "API/TableColumn.h"

#include "Numeric/FunctionDomain1D.h"
#include "Numeric/FunctionValues.h"

#include "API/AlgorithmFactory.h"
#include "API/WorkspaceFactory.h"

#include "Kernel/CommonProperties.h"

namespace Numeric
{

  DECLARE_ALGORITHM(ChebfunToTable);

/**
 * Constructor.
 */
ChebfunToTable::ChebfunToTable()
{
  declareWorkspace("InputWorkspace");
  declareWorkspace("OutputWorkspace");
  declareInt("N");
  declareBool("DropXInf", true);
}

void ChebfunToTable::exec()
{
  ChebfunWorkspace_sptr cws = getClass("InputWorkspace");

  Numeric::FunctionDomain1D_sptr domain;

  size_t n = (int)get("N");

  if (n < 2)
  {// if n has default value (0) use the x-points of the chebfuns
    domain = cws->fun().createDomainFromXPoints();
    n = domain->size();
  }
  else
  {// otherwise create a regular comb
    domain = cws->fun().createDomain( n );
  }

  Numeric::FunctionValues values( *domain );
  cws->fun().function(*domain, values);

  auto tws = API::TableWorkspace_ptr(dynamic_cast<API::TableWorkspace*>(
    API::WorkspaceFactory::instance().create("TableWorkspace"))
    );

  tws->addColumn("double","X");
  tws->addColumn("double","Y");
  tws->setRowCount(n);
  auto xColumn = static_cast<API::TableColumn<double>*>(tws->getColumn("X").get());
  xColumn->asNumeric()->setPlotRole(API::NumericColumn::X);
  auto& x = xColumn->data();
  auto yColumn = static_cast<API::TableColumn<double>*>(tws->getColumn("Y").get());
  yColumn->asNumeric()->setPlotRole(API::NumericColumn::Y);
  auto& y = yColumn->data();
  
  for(size_t i = 0; i < domain->size(); ++i)
  {
    x[i] = (*domain)[i];
    y[i] = values.getCalculated(i);
  }

  bool dropXInf = get("DropXInf");
  if ( dropXInf )
  {
    if ( fabs( x.front() ) == inf )
    {
      tws->removeRow( 0 );
    }
    if ( fabs( x.back() ) == inf )
    {
      tws->removeRow( tws->rowCount() - 1 );
    }
  }

  setProperty("OutputWorkspace",tws);
}

} // Numeric
