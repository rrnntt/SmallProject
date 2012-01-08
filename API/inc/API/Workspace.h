#ifndef API_WORKSPACE_H
#define API_WORKSPACE_H

#include "API/DllExport.h"
#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>

namespace API
{

class API_EXPORT Workspace
{
public:
  Workspace();
  Workspace(const Workspace&);
  virtual ~Workspace();
  std::string name() const {return m_name;}
  void setName(const std::string& nm)  {m_name = nm;}
  void modified();
protected:
  std::string m_name;
};

typedef boost::shared_ptr<Workspace> Workspace_ptr;

} // API

#endif // API_WORKSPACE_H
