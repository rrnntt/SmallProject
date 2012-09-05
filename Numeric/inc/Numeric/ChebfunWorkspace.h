#ifndef NUMERIC_CHEBFUNWORKSPACE_H
#define NUMERIC_CHEBFUNWORKSPACE_H

#include "Numeric/DllExport.h"
#include "API/Workspace.h"
#include "Numeric/ChebFunction.h"

namespace Numeric
{

/**
 * 
 */
class NUMERIC_EXPORT ChebfunWorkspace: public API::Workspace
{
public:
  ChebfunWorkspace();
  ChebfunWorkspace(const ChebFunction& fun);
  ChebfunWorkspace(const chebfun& fun);
  ~ChebfunWorkspace(){}
  virtual std::string id()const {return "ChebfunWorkspace";}
  ChebFunction& fun(){return m_fun;}
private:
  ChebFunction m_fun;
};

/// typedef shared pointer
typedef boost::shared_ptr<ChebfunWorkspace> ChebfunWorkspace_sptr;
typedef boost::shared_ptr<const ChebfunWorkspace> ChebfunWorkspace_const_sptr;

} // namespace Numeric

#endif // NUMERIC_CHEBFUNWORKSPACE_H
