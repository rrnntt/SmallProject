#include "Teach/SimulateParticle.h"
#include "Numeric/ChebfunWorkspace.h"

#include "API/AlgorithmFactory.h"

#include "Kernel/CommonProperties.h"

namespace Teach
{

DECLARE_ALGORITHM(SimulateParticle);

/// Constructor. Declare algorithm properties.
SimulateParticle::SimulateParticle()
{
  declareWorkspace("XProj");
}

/// Execute algorithm.
void SimulateParticle::exec()
{
  Numeric::ChebfunWorkspace *cws = new Numeric::ChebfunWorkspace;
  setProperty("XProj", boost::shared_ptr<Numeric::ChebfunWorkspace>(cws));
}

} // namespace Teach
