#include "API/Algorithm.h"
#include "API/WorkspaceManager.h"
#include "Kernel/Logger.h"

#include <stdexcept>
#include <algorithm>

namespace API
{
  Algorithm::Algorithm():g_log(Kernel::Logger::get("Algorithm"))
  {
  }
  void Algorithm::execute()
  {
    try
    {
      this->exec();
    }
    catch(std::exception& e)
    {
      std::cerr << "Algorithm error: " << e.what() << std::endl;
    }
  }

  /// Send a progress report to the listeners
  void Algorithm::sendProgress(size_t current, size_t maximum)
  {
    for(auto it = m_listeners.begin(); it != m_listeners.end(); ++it)
    {
      (**it).algorithmProgress(this, current, maximum);
    }
  }

  /**
   * Declare a workspace property.
   */
  void Algorithm::declareWorkspace(const std::string& propName, Kernel::Property::Direction dir)
  {
      declareClass( propName, &WorkspaceManager::instance(), dir );
  }

  /// Set a value of a workspace property
  void Algorithm::setClassProperty(const std::string& name, boost::shared_ptr<Kernel::PropertyClass> value)
  {
    auto ws = boost::dynamic_pointer_cast<Workspace>( value );
    if ( ws )
    {
      Workspace_ptr oldValue = getClass(name);
      if ( oldValue )
      {
        const std::string wsName = oldValue->name();
        if ( !wsName.empty() )
        {
          WorkspaceManager::instance().addOrReplace( wsName, ws );
        }
      }
    }
    Kernel::PropertyManager::setClassProperty( name, value );
  }

} // API
