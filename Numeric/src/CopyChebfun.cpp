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
  declareClass("InputWorkspace","WorkspaceManager");
  declareClass("OutputWorkspace","WorkspaceManager");
}

void CopyChebfun::exec()
{
  ChebfunWorkspace_sptr inws = getClass("InputWorkspace");

  ChebfunWorkspace *cws = new ChebfunWorkspace;
  cws->fun(0) = inws->fun(0);

  setProperty("OutputWorkspace", boost::shared_ptr<ChebfunWorkspace>(cws) );

}

} // Numeric
