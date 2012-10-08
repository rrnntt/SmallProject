#include "Numeric/MakeQuadrature.h"
#include "Numeric/Quadrature.h"
#include "Numeric/JacobiPolynomial.h"

#include "API/AlgorithmFactory.h"

#include "Kernel/CommonProperties.h"

namespace Numeric
{

DECLARE_ALGORITHM(MakeQuadrature);

/// Constructor. Declare algorithm properties.
MakeQuadrature::MakeQuadrature()
{
  declareWorkspace("Quadrature", Kernel::Property::Output);
  // Possible type values:
  // Custom 
  // Laguerre
  // Jacobi (includes Legendre)
  // Hermite
  declareString("Type","Custom");
  declareInt("N",10);
}

void MakeQuadrature::applyProperty(const std::string& name)
{
  if ( name == "Chebyshev" )
  {
    declareDouble("StartX",-1.0);
    declareDouble("EndX",1.0);
  }
}

/// Execute algorithm.
void MakeQuadrature::exec()
{
  std::string type = get("Type");
  if ( type == "Chebyshev" )
  {
    makeChebyshev();
  }
}

void MakeQuadrature::makeChebyshev()
{
  const double startX = get("StartX");
  const double endX = get("EndX");
  const int n = getN("N");
  JacobiPolynomial jac( 0.5, 0.5, n );
}

} // namespace Numeric
