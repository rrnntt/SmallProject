#ifndef NUMERIC_LAGUERREWEIGHT_H
#define NUMERIC_LAGUERREWEIGHT_H

#include "Numeric/DllExport.h"
#include "Numeric/IFunction1D.h"
#include "Numeric/ParamFunction.h"

namespace Numeric
{

/**
 * The weight function for the generalized Laguerre polynomials: x^alpha * exp(-x)
 */
class NUMERIC_EXPORT LaguerreWeight: public IFunction1D, public ParamFunction
{
public:
  LaguerreWeight();
  ~LaguerreWeight();
  /// Returns the function's name
  virtual std::string name()const {return "LaguerreWeight";}
  /// Function you want to fit.
  virtual void function1D(double* out, const double* xValues, const size_t nData)const;
  /// Derivatives of function with respect to active parameters
  virtual void functionDeriv1D(Jacobian* out, const double* xValues, const size_t nData);
};

/**
 * The weight function for the generalized Laguerre polynomials: x^alpha * exp(-x)
 */
class NUMERIC_EXPORT LaguerreWeightDerivative: public IFunction1D, public ParamFunction
{
public:
  LaguerreWeightDerivative();
  /// Returns the function's name
  virtual std::string name()const {return "LaguerreWeightDerivative";}
  /// Function you want to fit.
  virtual void function1D(double* out, const double* xValues, const size_t nData)const;
};

} // Numeric

#endif // NUMERIC_LAGUERREWEIGHT_H
