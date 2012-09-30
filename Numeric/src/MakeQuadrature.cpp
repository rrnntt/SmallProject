#include "Numeric/MakeQuadrature.h"
#include "Numeric/Quadrature.h"

#include "API/AlgorithmFactory.h"

#include "Kernel/CommonProperties.h"

namespace Numeric
{

DECLARE_ALGORITHM(MakeQuadrature);

/// Constructor. Declare algorithm properties.
MakeQuadrature::MakeQuadrature()
{
  // Possible type values:
  // Custom 
  // Laguerre
  // Jacobi (includes Legendre)
  // Hermite
  declareString("Type","Custom");
  declareWorkspace("Quadrature", Kernel::Property::Output);
}

/// Execute algorithm.
void MakeQuadrature::exec()
{
}

} // namespace Numeric
