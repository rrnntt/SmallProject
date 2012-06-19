#include "Numeric/CopyChebfun.h"
#include "Numeric/ChebfunWorkspace.h"

#include "API/AlgorithmFactory.h"
#include "API/WorkspaceFactory.h"

#include "Kernel/CommonProperties.h"

namespace Numeric
{

  DECLARE_ALGORITHM(CopyChebfun);

/**
 * Constructor.
 */
CopyChebfun::CopyChebfun()
{
  declareWorkspace("InputWorkspace");
  declareWorkspace("OutputWorkspace");
}

void CopyChebfun::exec()
{
  ChebfunWorkspace_sptr inws = getClass("InputWorkspace");

  ChebfunWorkspace *cws = new ChebfunWorkspace;
  cws->fun(0) = inws->fun(0);

  setProperty("OutputWorkspace", boost::shared_ptr<ChebfunWorkspace>(cws) );

}

} // Numeric
