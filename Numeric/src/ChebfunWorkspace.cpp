#include "Numeric/ChebfunWorkspace.h"
#include "API/WorkspaceFactory.h"

#include <algorithm>

namespace Numeric
{

DECLARE_WORKSPACE(ChebfunWorkspace);

ChebfunWorkspace::ChebfunWorkspace():
ChebFunction()
{
}

ChebfunWorkspace::ChebfunWorkspace(const ChebFunction& fun):
ChebFunction(fun)
{
}

ChebfunWorkspace::ChebfunWorkspace(const chebfun& fun):
ChebFunction(fun)
{
}


} // namespace Numeric
