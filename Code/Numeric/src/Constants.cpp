#include "Numeric/Constants.h"

#define _USE_MATH_DEFINES
#include <limits>
#include <cmath>

namespace Numeric
{

  const double inf = std::numeric_limits<double>::infinity();
  const double minf = -inf;
  const double pi = M_PI;
  const double sqrt_2 = M_SQRT2;

  const double mass_p = 1.672621777e-27; // kg
  const double mass_e = 9.10938291e-31; // kg
  const double planck_h = 6.62606957e-34; // J*s
  const double planck_hbar = 1.054571726e-34; // J*s
  const double light_speed = 299792458.0; // m/s
  const double angstrom = 1e-10; // m

} // Numeric
