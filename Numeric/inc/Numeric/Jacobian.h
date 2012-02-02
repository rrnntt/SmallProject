#ifndef NUMERIC_JACOBIAN_H
#define NUMERIC_JACOBIAN_H

#include "Numeric/DllExport.h"

#include <stdexcept>

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

  ///@cond do not document
  /**  Add number to all iY (data) Jacobian elements for a given iP (parameter)
  *   @param value :: Value to add
  */
  virtual void addNumberToColumn(const double& value, const size_t& iActiveP) 
  {
    (void)value; (void)iActiveP; // Avoid compiler warning
    throw std::runtime_error("No addNumberToColumn() method of Jacobian provided");
  }
  ///@endcond

  /// Virtual destructor
  virtual ~Jacobian() {};
protected:
};

} // namespace Numeric

#endif // NUMERIC_JACOBIAN_H
