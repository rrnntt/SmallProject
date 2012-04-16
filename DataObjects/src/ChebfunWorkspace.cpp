#include "DataObjects/ChebfunWorkspace.h"
#include "API/WorkspaceFactory.h"

namespace DataObjects
{

DECLARE_WORKSPACE(ChebfunWorkspace);

ChebfunWorkspace::ChebfunWorkspace():
m_fun(new Numeric::chebfun)
{
}

} // namespace DataObjects
