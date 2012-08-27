#include "Goblin/Diatom.h"

#include "Numeric/Constants.h"
#include "Numeric/IFunction.h"
#include "Numeric/FunctionFactory.h"

#include "API/AlgorithmFactory.h"
#include "API/TableWorkspace.h"

#include "Kernel/CommonProperties.h"

#include <stdexcept>

namespace Goblin
{

DECLARE_ALGORITHM(Diatom);

/// Constructor. Declare algorithm properties.
Diatom::Diatom()
{
  declareDouble("mu");
  declareString("Vpot");
  declareInt("nmax",10);
  declareInt("Jmax",10);
}

/// Execute algorithm.
void Diatom::exec()
{
  double mu = get("mu");
  if ( mu <= 0 )
  {
    throw std::invalid_argument("mu cannot be negative or zero");
  }
  std::string funStr = get("Vpot");
  Numeric::IFunction_sptr Vpot = Numeric::FunctionFactory::instance().createFitFunction(funStr);
  int nmax = get("nmax");
  int Jmax = get("Jmax");
}

} // namespace Goblin
