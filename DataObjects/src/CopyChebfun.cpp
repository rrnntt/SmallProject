#include "DataObjects/CopyChebfun.h"
#include "DataObjects/ChebfunWorkspace.h"

#include "API/AlgorithmFactory.h"
#include "API/WorkspaceProperty.h"
#include "API/WorkspaceFactory.h"

#include "Kernel/CommonProperties.h"

namespace DataObjects
{

  DECLARE_ALGORITHM(CopyChebfun);

/**
 * Constructor.
 */
CopyChebfun::CopyChebfun()
{
  declare("InputWorkspace",new API::WorkspaceProperty(Kernel::Property::Input));
  declare("OutputWorkspace",new API::WorkspaceProperty(Kernel::Property::Output));
}

void CopyChebfun::exec()
{
  API::WorkspaceProperty inWS = get("InputWorkspace").as<API::WorkspaceProperty>();
  auto inws = inWS.to<ChebfunWorkspace>();
  if (!inws)
  {
    throw std::runtime_error("InputWorkspace property is not a ChebfunWorkspace");
  }

  API::WorkspaceProperty outWS = get("OutputWorkspace").as<API::WorkspaceProperty>();

  ChebfunWorkspace *cws = new ChebfunWorkspace;
  cws->fun() = inws->fun();

  outWS = boost::shared_ptr<ChebfunWorkspace>(cws);

}

} // DataObjects
