#ifndef TEACH_GEOM2DWORKSPACE_H
#define TEACH_GEOM2DWORKSPACE_H

#include "Teach/DllExport.h"

#include "API/Workspace.h"

namespace Teach
{

class TEACH_EXPORT Geom2DWorkspace: public API::Workspace
{
public:
  Geom2DWorkspace();
  ~Geom2DWorkspace();
  virtual std::string id()const{return "Geom2DWorkspace";}
};

} // TEACH

#endif // TEACH_GEOM2DWORKSPACE_H
