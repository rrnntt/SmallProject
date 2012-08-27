#include "Numeric/Constants.h"

#define _USE_MATH_DEFINES
#include <limits>
#include <cmath>

namespace Numeric
{

  const double inf = std::numeric_limits<double>::infinity();
  const double minf = -inf;
  const double pi = M_PI;

} // Numeric
