#ifndef NUMERIC_COSTFUNCTION_H
#define NUMERIC_COSTFUNCTION_H

#include "Numeric/DllExport.h"

#include <boost/shared_ptr.hpp>
#include <vector>

namespace Numeric
{

class NUMERIC_EXPORT CostFunction
{
public:
  CostFunction(){}
  virtual ~CostFunction(){}
  /// Get i-th parameter
  virtual double getParameter(size_t i)const = 0;
  /// Set i-th parameter
  virtual void setParameter(size_t, const double& value) = 0;
  /// Total number of parameters
  virtual size_t nParams()const = 0;
  /// Evaluate the function
  virtual double eval() const = 0;
  /// Evaluate the derivatives
  virtual void deriv(std::vector<double>&) const = 0;
  /// Evaluate the function and the derivatives
  virtual double evalAndDeriv(std::vector<double>&) const = 0;
};

typedef boost::shared_ptr<CostFunction> CostFunction_ptr;

} // namespace Numeric
#endif // NUMERIC_COSTFUNCTION_H
