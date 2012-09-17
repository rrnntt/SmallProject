#include "Numeric/CreateQuadratureChebfuns.h"
#include "Numeric/ChebfunWorkspace.h"

#include "API/AlgorithmFactory.h"

#include "Kernel/CommonProperties.h"

namespace Numeric
{

DECLARE_ALGORITHM(CreateQuadratureChebfuns);

/// Constructor. Declare algorithm properties.
CreateQuadratureChebfuns::CreateQuadratureChebfuns()
{
  // input quadrature workspace with format of the 
  // output from MakeQuadratureScheme
  declareWorkspace("Quadrature");
  // number of points in the result chebfuns
  declareInt("NPoints",100);
}

/// Execute algorithm.
void CreateQuadratureChebfuns::exec()
{
}

} // namespace Numeric
