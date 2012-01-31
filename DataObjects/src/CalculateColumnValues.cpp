#include "DataObjects/CalculateColumnValues.h"
#include "DataObjects/TableWorkspace.h"

#include "API/AlgorithmFactory.h"
#include "API/WorkspaceProperty.h"

#include "Kernel/CommonProperties.h"

#include "Formula/Expression.h"

namespace DataObjects
{

DECLARE_ALGORITHM(CalculateColumnValues);

CalculateColumnValues::CalculateColumnValues()
{
  declare("Workspace",new API::WorkspaceProperty(Kernel::Property::InOut));
  declare("Column",new Kernel::StringProperty);
  declare("Formula",new Kernel::StringProperty);
}

/**
 * Retrieve the input table workspace from its property and validat it.
 */
boost::shared_ptr<TableWorkspace> CalculateColumnValues::getTableWorkspace()
{
    API::WorkspaceProperty wsProp = get("Workspace").as<API::WorkspaceProperty>();
    std::string wsName = static_cast<std::string>(wsProp);
    if (wsName.empty())
    {// TODO: this must be done by a validator
      throw std::runtime_error("Property Workspace was not set");
    }
    auto tws = wsProp.to<TableWorkspace>();

    if (!tws)
    {
      throw std::runtime_error("Workspace property is not a TableWorkspace");
    }

    if (tws->columnCount() == 0)
    {
      throw std::runtime_error("Empty table found");
    }
    return tws;
}

void CalculateColumnValues::exec()
{
  TableWorkspace_ptr tws = getTableWorkspace();
  std::string columnName = get("Column");
}

} // namespace DataObjects
