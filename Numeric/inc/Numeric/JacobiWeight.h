#ifndef NUMERIC_JACOBIWEIGHT_H
#define NUMERIC_JACOBIWEIGHT_H

#include "Numeric/DllExport.h"
#include "Numeric/IFunction1D.h"
#include "Numeric/ParamFunction.h"

namespace Numeric
{

/**
 * The weight function for the Jacobi polynomials: (1 - x)^alpha * (1 + x)^beta
 */
class NUMERIC_EXPORT JacobiWeight: public IFunction1D, public ParamFunction
{
public:
  JacobiWeight();
  ~JacobiWeight();
  /// Returns the function's name
  virtual std::string name()const {return "JacobiWeight";}
  /// Function you want to fit.
  virtual void function1D(double* out, const double* xValues, const size_t nData)const;
  /// Derivatives of function with respect to active parameters
  virtual void functionDeriv1D(Jacobian* out, const double* xValues, const size_t nData);
};

} // Numeric

#endif // NUMERIC_JACOBIWEIGHT_H
