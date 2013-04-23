#include "Numeric/AnalyseBasis.h"
#include "Numeric/ChebfunVector.h"
#include "Numeric/ChebFunction.h"
#include "Numeric/ChebfunWorkspace.h"

#include "API/AlgorithmFactory.h"

#include "Kernel/CommonProperties.h"

namespace Numeric
{

DECLARE_ALGORITHM(AnalyseBasis);

/// Constructor. Declare algorithm properties.
AnalyseBasis::AnalyseBasis()
{
  declareWorkspace("Basis");
  declareWorkspace("Out");
}

/// Execute algorithm.
void AnalyseBasis::exec()
{
  ChebfunVector_sptr basis = getClass("Basis");
  if ( basis->size() < 2 ) return;
  ChebfunWorkspace_sptr fun( new ChebfunWorkspace( basis->cfun(1) ) );
  fun->fun() /= basis->cfun(0);
  setProperty( "Out", fun );
}

} // namespace Numeric
