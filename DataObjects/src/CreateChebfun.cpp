#include "DataObjects/CreateChebfun.h"
#include "DataObjects/ChebfunWorkspace.h"
#include "API/AlgorithmFactory.h"
#include "API/WorkspaceProperty.h"

namespace DataObjects
{

DECLARE_ALGORITHM(CreateChebfun);

CreateChebfun::CreateChebfun()
{
  declare("OutputWorkspace",new API::WorkspaceProperty(Kernel::Property::Output));
}

void CreateChebfun::exec()
{
}

} // namespace DataObjects
