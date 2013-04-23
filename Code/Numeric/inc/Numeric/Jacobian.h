#ifndef NUMERIC_JACOBIAN_H_
#define NUMERIC_JACOBIAN_H_

//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Numeric/DllExport.h"

namespace Numeric
{
/** 
    Represents the Jacobian in IFitFunction::functionDeriv.

*/
class NUMERIC_EXPORT Jacobian
{
public:
  /**  Set a value to a Jacobian matrix element.
  *   @param iY :: The index of a data point.
  *   @param iP :: The index of a declared parameter.
  *   @param value :: The derivative value.
  */
  virtual void set(size_t iY, size_t iP, double value) = 0;

  /**  Get the value to a Jacobian matrix element.
  *   @param iY :: The index of a data point.
  *   @param iP :: The index of a declared parameter.
  */
  virtual double get(size_t iY, size_t iP) = 0;

  /// Virtual destructor
  virtual ~Jacobian() {};
protected:
};

} // namespace Numeric

#endif /*NUMERIC_JACOBIAN_H_*/
