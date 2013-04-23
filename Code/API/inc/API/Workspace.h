#ifndef API_WORKSPACE_H
#define API_WORKSPACE_H

#include "API/DllExport.h"
#include "Kernel/PropertyClass.h"

#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>

namespace API
{

/**
 * A base class for all workspaces. Workspaces are more or less complex data structures
 * that can be passed in and out of algorithms.
 */
class API_EXPORT Workspace: public Kernel::PropertyClass
{
public:
  virtual ~Workspace(){}
  /* Implemented virual methods of PropertyClass */
  std::string asString() const {return m_name;}
  /// Store this workspace in WorkspaceManager
  void store(const std::string& nm);

  /* Own public virtual methods */
  /// ID of a workspace class. WorkspaceFactory creates workspace instances from ids.
  virtual std::string id()const = 0;

  /// Name the workspace stored with in WorkspaceManager
  std::string name() const {return m_name;}
  /// Inform the world that it's modified
  void modified();
private:
  /// Setting the name. Only WorkspaceManager can do it. Having a non-empty name means
  /// the workspace is in WorkspaceManager
  void setName(const std::string& nm)  {m_name = nm;}
  /// Name of this workspace in WorkspaceManager. If empty it's not managed.
  std::string m_name;
  friend class WorkspaceManager;
};

/// Shared pointer to a workspace
typedef boost::shared_ptr<Workspace> Workspace_ptr;

} // API

#endif // API_WORKSPACE_H
