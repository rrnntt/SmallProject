#ifndef DATAOBJECTS_CHEBFUNWORKSPACE_H
#define DATAOBJECTS_CHEBFUNWORKSPACE_H

#include "DataObjects/DllExport.h"
#include "API/Workspace.h"
#include "Numeric/Chebfun.h"

namespace DataObjects
{

class DATAOBJECTS_EXPORT ChebfunWorkspace: public API::Workspace
{
public:
  ChebfunWorkspace();
  ~ChebfunWorkspace(){}
  virtual std::string id()const {return "ChebfunWorkspace";}
protected:
  Numeric::chebfun_ptr m_fun;
};

} // namespace DataObjects
#endif // DATAOBJECTS_CHEBFUNWORKSPACE_H
