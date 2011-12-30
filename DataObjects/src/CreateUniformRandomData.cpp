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
    declare("Workspace",new API::WorkspaceProperty(Kernel::Property::Output));
  }

  void CreateUniformRandomData::exec()
  {
    API::WorkspaceProperty wsProp = get("Workspace").as<API::WorkspaceProperty>();
    std::string wsName = static_cast<std::string>(wsProp);
    if (wsName.empty())
    {
      throw std::runtime_error("Property Workspace was not set");
    }
    TableWorkspace_ptr tws = TableWorkspace_ptr(static_cast<TableWorkspace*>(API::WorkspaceFactory::instance().create("TableWorkspace")));
    std::cerr << "CreateUniformRandomData\n";
    std::cerr << "Workspace " << static_cast<std::string>(wsProp) << std::endl;
    wsProp = tws;

  }

}  // DataObjects

