#include "Numeric/AnalyseBasis.h"
#include "Numeric/ChebfunVector.h"

#include "API/AlgorithmFactory.h"

#include "Kernel/CommonProperties.h"

namespace Numeric
{

DECLARE_ALGORITHM(AnalyseBasis);

/// Constructor. Declare algorithm properties.
AnalyseBasis::AnalyseBasis()
{
  declareWorkspace("Basis");
}

/// Execute algorithm.
void AnalyseBasis::exec()
{
  ChebfunVector_sptr basis = getClass("Bassis");
}

} // namespace Numeric
