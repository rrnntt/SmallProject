#include "Numeric/Minimizer.h"

namespace Numeric
{

Minimizer::Minimizer()
{
}

void Minimizer::setCostFunction(CostFunction_ptr cf)
{
  m_costFunction = cf;
}

} // namespace Numeric
