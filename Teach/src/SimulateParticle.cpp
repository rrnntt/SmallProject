#include "Teach/SimulateParticle.h"
#include "DataObjects/ChebfunWorkspace.h"

#include "API/AlgorithmFactory.h"
#include "API/WorkspaceProperty.h"

#include "Kernel/CommonProperties.h"

namespace Teach
{

DECLARE_ALGORITHM(SimulateParticle);

/// Constructor. Declare algorithm properties.
SimulateParticle::SimulateParticle()
{
  declare("XProj",new API::WorkspaceProperty(Kernel::Property::Output));
}

/// Execute algorithm.
void SimulateParticle::exec()
{
  API::WorkspaceProperty wsProp = get("OutputWorkspace").as<API::WorkspaceProperty>();
  DataObjects::ChebfunWorkspace *cws = new DataObjects::ChebfunWorkspace;
  cws->set(10,-3.3,3.3);
  cws->fun().fit(sin);

  wsProp = boost::shared_ptr<DataObjects::ChebfunWorkspace>(cws);
}

} // namespace Teach
