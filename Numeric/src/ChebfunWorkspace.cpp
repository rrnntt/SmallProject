#include "Numeric/ChebfunWorkspace.h"
#include "API/WorkspaceFactory.h"

#include <algorithm>

namespace Numeric
{

DECLARE_WORKSPACE(ChebfunWorkspace);

ChebfunWorkspace::ChebfunWorkspace():
Numeric::ChebFunction()
{
}

} // namespace Numeric
