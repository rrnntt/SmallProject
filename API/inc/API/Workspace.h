#ifndef API_WORKSPACE_H
#define API_WORKSPACE_H

#include "API/DllExport.h"
#include "Kernel/PropertyClass.h"

#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>

namespace API
{

class API_EXPORT Workspace: public Kernel::PropertyClass
{
public:
  virtual ~Workspace(){}
  virtual std::string id()const = 0;
  std::string asString() const {return m_name;}
  std::string name() const {return m_name;}
  void modified();
private:
  void setName(const std::string& nm)  {m_name = nm;}
  std::string m_name;
  friend class WorkspaceManager;
};

typedef boost::shared_ptr<Workspace> Workspace_ptr;

} // API

#endif // API_WORKSPACE_H
