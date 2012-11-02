#include "Numeric/MakeQuadrature.h"
#include "Numeric/Quadrature.h"
#include "Numeric/JacobiPolynomial.h"
#include "Numeric/Hermite.h"
#include "Numeric/Laguerre.h"
#include "Numeric/FunctionDomain1D.h"
#include "Numeric/FunctionValues.h"

#include "API/AlgorithmFactory.h"
#include "API/NumericColumn.h"

#include "Kernel/CommonProperties.h"

#include <boost/lexical_cast.hpp>

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
  declareBool("Normalize",true);
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
  else if ( type == "Laguerre" )
  {
    makeLaguerre();
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
  const bool norm = get("Normalize");
  Hermite H( n );
  if ( norm )
  {
    H.normalize();
  }

  makeQuadrature( H );
}

/**-------------------------------------------------
 * Make a Gauss-Laguerre quadrature.
 */
void MakeQuadrature::makeLaguerre()
{
  const int n = get("N");
  Laguerre H( 0, n );
  const bool norm = get("Normalize");
  if ( norm )
  {
    H.normalize();
  }

  makeQuadrature( H );
}

/**-------------------------------------------------
 * Make a quadrature given a Polynomial.
 * @param P :: A polynomial to use to make the quadrature.
 */
void MakeQuadrature::makeQuadrature(const Polynomial& P)
{
  auto& r = P.getRoots();
  auto& w = P.getWeights();
  const size_t n = r.size();

  auto quad = new Quadrature;
  quad->setRowCount( int(n) );
  quad->addDoubleColumn("r", API::NumericColumn::X);
  auto& rc = quad->getDoubleData("r");
  rc = r;
  quad->addDoubleColumn("w", API::NumericColumn::Y);
  auto& wc = quad->getDoubleData("w");
  wc = w;

  FunctionDomain1DView domain( r );
  FunctionValues values( domain );
  std::vector<double> wgt;
  std::vector<double> wgtDeriv;
  P.weightFunction()->function( domain, values );
  values.copyToStdVector( wgt );
  P.weightDerivative()->function( domain, values );
  values.copyToStdVector( wgtDeriv );

  quad->addDoubleColumn("weight", API::NumericColumn::Y);
  auto& wgtc = quad->getDoubleData("weight");
  wgtc = wgt;
  quad->addDoubleColumn("deriv", API::NumericColumn::Y);
  auto& derc = quad->getDoubleData("deriv");
  derc = wgtDeriv;

  Quadrature::FuncVector fvec( n );
  Quadrature::FuncVector dvec( n );
  for(size_t i = 0; i < n; ++i)
  {
    std::string colInd = boost::lexical_cast<std::string>( i );
    quad->addDoubleColumn("f"+colInd, API::NumericColumn::Y);
    fvec[i] = &quad->getDoubleData("f"+colInd);
    quad->addDoubleColumn("d"+colInd, API::NumericColumn::Y);
    dvec[i] = &quad->getDoubleData("d"+colInd);
  }
  P.calcPolyValues( fvec, dvec );
  quad->init();

  setClassProperty( "Quadrature", API::TableWorkspace_ptr( quad ) );
}

} // namespace Numeric
