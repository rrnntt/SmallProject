#include "Numeric/ChebfunWorkspace.h"
#include "API/WorkspaceFactory.h"

#include <algorithm>

namespace Numeric
{

DECLARE_WORKSPACE(ChebfunWorkspace);

ChebfunWorkspace::ChebfunWorkspace():
m_fun()
{
}

ChebfunWorkspace::ChebfunWorkspace(const ChebFunction& fun):
m_fun(fun)
{
}

ChebfunWorkspace::ChebfunWorkspace(const chebfun& fun):
m_fun(fun)
{
}


} // namespace Numeric
