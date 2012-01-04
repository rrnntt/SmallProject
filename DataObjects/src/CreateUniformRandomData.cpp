#include "DataObjects/CreateUniformRandomData.h"
#include "DataObjects/TableWorkspace.h"
#include "API/AlgorithmFactory.h"
#include "API/WorkspaceProperty.h"
#include "API/WorkspaceFactory.h"

namespace DataObjects
{

  DECLARE_ALGORITHM(CreateUniformRandomData);

  CreateUniformRandomData::CreateUniformRandomData()
  {
    declare("Workspace",new API::WorkspaceProperty(Kernel::Property::InOut));
  }

  void CreateUniformRandomData::exec()
  {
    API::WorkspaceProperty wsProp = get("Workspace").as<API::WorkspaceProperty>();
    std::string wsName = static_cast<std::string>(wsProp);
    if (wsName.empty())
    {
      throw std::runtime_error("Property Workspace was not set");
    }
    auto tws = wsProp.to<TableWorkspace_ptr>();

    if (tws)
    {
      std::cerr << "Table " << tws->rowCount() << " x " << tws->columnCount() << std::endl;
    }
    else
    {
      std::cerr << "Table not set\n";
    }

  }

}  // DataObjects

