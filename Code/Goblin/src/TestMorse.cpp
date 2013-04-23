#include "Goblin/TestMorse.h"

#include "Numeric/FunctionFactory.h"
#include "Numeric/IFunction.h"
#include "Numeric/ChebfunVector.h"

#include "API/AlgorithmFactory.h"
#include "API/TableWorkspace.h"

#include "Kernel/CommonProperties.h"

#include <boost/lexical_cast.hpp>

namespace Goblin
{

DECLARE_ALGORITHM(TestMorse);

/// Constructor. Declare algorithm properties.
TestMorse::TestMorse()
{
  declareDouble("mu");  // reduced mass
  declareDouble("De");  // The dissociation energy
  declareDouble("a");   // The a-constant
  declareDouble("re");  // The equilibrium distance
  declareInt("NBasis",10);// The basis size
  declareWorkspace("Quad");
  declareWorkspace("Basis");
}

/// Execute algorithm.
void TestMorse::exec()
{
  const int nBasis = get("NBasis");
  auto vpot = Numeric::FunctionFactory::instance().createFitFunction("UserFunction1D(Formula=De*( 1-exp(-(x-re)*a) )^2 )");
  const double mu = get("mu");
  const double De = get("De");
  const double a = get("a");
  const double re = get("re");
  vpot->setParameter("De",De);
  vpot->setParameter("a",a);
  vpot->setParameter("re",re);

  const double alpha = 2.0 * De * a * a;
  const double beta  = 1.0 / mu;

  auto f0Fun = Numeric::FunctionFactory::instance().createFitFunction( "UserFunction1D(Formula=x*exp(-a*(x-c)^2))" );
  f0Fun->setParameter("a",sqrt(alpha/beta));
  f0Fun->setParameter("c",re);
  std::string intervalStr = boost::lexical_cast<std::string>(0.00001)+","+boost::lexical_cast<std::string>(4*re);
  auto alg = API::AlgorithmFactory::instance().createAlgorithm("MakeQuadratureScheme");
  alg->setProperty("N",nBasis);
  alg->setProperty("Interval",intervalStr);
  alg->setProperty("Function",f0Fun);
  alg->execute();

  API::TableWorkspace_ptr quadWS = alg->getClass("Quadrature");
  Numeric::ChebfunVector_sptr basis = alg->getClass("Basis");

  setProperty("Quad",quadWS);
  setProperty("Basis",basis);
}

} // namespace Goblin
