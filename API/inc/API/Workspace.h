#ifndef API_WORKSPACE_H
#define API_WORKSPACE_H

#include "API/DllExport.h"
#include <boost/shared_ptr.hpp>
#include <vector>

namespace API
{

class API_EXPORT Workspace
{
public:
  Workspace();
  Workspace(const Workspace&);
  ~Workspace();
};

typedef boost::shared_ptr<Workspace> Workspace_ptr;

} // API

#endif // API_WORKSPACE_H
