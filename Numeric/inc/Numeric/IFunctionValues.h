#ifndef NUMERIC_IFUNCTIONVALUES_H
#define NUMERIC_IFUNCTIONVALUES_H

#include "Numeric/DllExport.h"

namespace Numeric
{

class NUMERIC_EXPORT IFunctionValues
{
public:
  /// Default constructor.
  virtual ~IFunctionValues(){}
  /// Return the number of values
  virtual size_t size() const  = 0;
  /// Get a pointer to calculated data at index i
  virtual double* getPointerToCalculated(size_t i) = 0;
  /// Set all calculated values to zero
  virtual void zeroCalculated() = 0;
  /// set all calculated values to same number
  virtual void setCalculated(double value) = 0;

protected:
  /// Copy calculated values to a buffer
  /// @param to :: Pointer to the buffer, it must be large enough
  virtual void copyTo(double* to) const = 0;
  /// Add calculated values to values in a buffer and save result to the buffer
  /// @param to :: Pointer to the buffer, it must be large enough
  virtual void add(double* to) const = 0;
  /// Multiply calculated values by values in a buffer and save result to the buffer
  /// @param to :: Pointer to the buffer, it must be large enough
  virtual void multiply(double* to) const = 0;

};

/// typedef for a shared pointer
typedef boost::shared_ptr<IFunctionValues> IFunctionValues_ptr;

} // NUMERIC

#endif // NUMERIC_IFUNCTIONVALUES_H
