#ifndef NUMERIC_CONSTANTS_H
#define NUMERIC_CONSTANTS_H

#include "Numeric/DllExport.h"

namespace Numeric
{

  /// Positive infinity
  extern NUMERIC_EXPORT const double inf;
  /// Negative infinity
  extern NUMERIC_EXPORT const double minf;
  /// PI
  extern NUMERIC_EXPORT const double pi;

  /// Proton mass in kg
  extern NUMERIC_EXPORT const double mass_p;
  /// Electron mass in kg
  extern NUMERIC_EXPORT const double mass_e;
  /// Planck constant in J*s
  extern NUMERIC_EXPORT const double planck_h;
  /// Planck constant over 2*pi in J*s
  extern NUMERIC_EXPORT const double planck_hbar;
  /// Speed of light in vacuum in km/s
  extern NUMERIC_EXPORT const double light_speed;

} // NUMERIC

#endif // NUMERIC_CONSTANTS_H