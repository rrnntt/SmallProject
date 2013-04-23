#include "Numeric/DiffChebfun.h"
#include "Numeric/ChebfunWorkspace.h"

#include "API/AlgorithmFactory.h"

#include "Kernel/CommonProperties.h"

namespace Numeric
{

DECLARE_ALGORITHM(DiffChebfun);

/// Constructor. Declare algorithm properties.
DiffChebfun::DiffChebfun()
{
  // Input chebfun
  declareWorkspace("Chebfun");
  // Output derivative chebfun
  declareWorkspace("Derivative");
  // Order of the derivative
  declareInt("N",1);
}

/// Execute algorithm.
void DiffChebfun::exec()
{
  ChebfunWorkspace_sptr fun = getClass("Chebfun");
  int n = get("N");
  ChebFunction der;
  der.fromDerivative( fun->fun() );
  for(int i=1; i < n; ++i)
  {
    ChebFunction tmp(der);
    der.fromDerivative( tmp );
  }
  ChebfunWorkspace* ws = new ChebfunWorkspace( der );
  setProperty( "Derivative", ChebfunWorkspace_sptr( ws ) );
}

} // namespace Numeric
