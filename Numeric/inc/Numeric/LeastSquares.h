#ifndef NUMERIC_LEASTSQUARES_H
#define NUMERIC_LEASTSQUARES_H

#include "Numeric/DllExport.h"
#include "Numeric/CostFunction.h"
#include "Numeric/IFunction.h"
#include "Numeric/FunctionDomain.h"

namespace Numeric
{

class NUMERIC_EXPORT LeastSquares: public CostFunction
{
public:
  LeastSquares();
  ~LeastSquares(){}
  /// Get i-th parameter
  virtual double getParameter(size_t i)const;
  /// Set i-th parameter
  virtual void setParameter(size_t, const double& value);
  /// Total number of parameters
  virtual size_t nParams()const;
  /// Evaluate the function
  virtual double eval() const;
  /// Evaluate the derivatives
  virtual void deriv(std::vector<double>&) const;
  /// Evaluate the function and the derivatives
  virtual double evalAndDeriv(std::vector<double>&) const;

  void setFunction(IFunction_ptr function){m_function = function;}
  void setDomain(FunctionDomain_ptr domain);
protected:
  IFunction_ptr m_function;
  FunctionDomain_ptr m_domain;
};

} // namespace Numeric
#endif // NUMERIC_LEASTSQUARES_H
