#include "Numeric/MakeQuadrature.h"
#include "Numeric/Quadrature.h"
#include "Numeric/JacobiPolynomial.h"
#include "Numeric/Hermite.h"
#include "Numeric/FunctionDomain1D.h"
#include "Numeric/FunctionValues.h"

#include "API/AlgorithmFactory.h"
#include "API/NumericColumn.h"

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

/**-------------------------------------------------
 * Apply property "Type". Different quadrature types may
 * require different additional properties.
 * @param name :: A name of a property to apply.
 */
void MakeQuadrature::applyProperty(const std::string& name)
{
  if ( name == "Type" )
  {
    std::string quad = get(name);
    if ( quad == "Chebyshev" )
    {
      declareDouble("StartX",-1.0);
      declareDouble("EndX",1.0);
    }
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
  else if ( type == "Hermite" )
  {
    makeHermite();
  }
}

/**-------------------------------------------------
 * Make a Gauss-Chebyshev quadrature.
 */
void MakeQuadrature::makeChebyshev()
{
  const double startX = get("StartX");
  const double endX = get("EndX");
  const int n = get("N");
  JacobiPolynomial jac( 0.5, 0.5, n );
}

/**-------------------------------------------------
 * Make a Gauss-Hermite quadrature.
 */
void MakeQuadrature::makeHermite()
{
  const int n = get("N");
  Hermite H( n );
  auto& r = H.getRoots();
  auto& w = H.getWeights();

  //---------------------------------------
  //  this is general for all quadratures.
  //---------------------------------------
  auto quad = new Quadrature;
  quad->setRowCount( n );
  quad->addDoubleColumn("r", API::NumericColumn::X);
  auto& rc = quad->getDoubleData("r");
  rc = r;
  quad->addDoubleColumn("w", API::NumericColumn::X);
  auto& wc = quad->getDoubleData("w");
  wc = w;
  FunctionDomain1DView domain( r );
  FunctionValues values( domain );
  std::vector<double> wgt;
  std::vector<double> wgtDeriv;
  H.weightFunction()->function( domain, values );
  values.copyToStdVector( wgt );
  H.weightDerivative()->function( domain, values );
  values.copyToStdVector( wgtDeriv );
}

} // namespace Numeric
