#include "Teach/SimulateParticle.h"
#include "Numeric/ChebfunWorkspace.h"

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
  API::WorkspaceProperty& wsProp = get("XProj").as<API::WorkspaceProperty>();
  Numeric::ChebfunWorkspace *cws = new Numeric::ChebfunWorkspace;
  cws->fun(0).set(10,-3.3,3.3);
  cws->fun(0).fit(sin);

  wsProp = boost::shared_ptr<Numeric::ChebfunWorkspace>(cws);
}

} // namespace Teach
