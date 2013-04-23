#ifndef NUMERIC_IFUNCTION1D_H_
#define NUMERIC_IFUNCTION1D_H_

//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Numeric/DllExport.h"
#include "Numeric/IFunction.h"
#include "Numeric/FunctionDomain1D.h"
#include "Kernel/Logger.h"

namespace Numeric
{

//----------------------------------------------------------------------
// Forward declaration
//----------------------------------------------------------------------
class Jacobian;
class ParameterTie;
class IConstraint;
class ParameterReference;

class NUMERIC_EXPORT IFunction1D: public virtual IFunction
{
public:

  /* Overidden methods */

  virtual void function(const FunctionDomain& domain,FunctionValues& values)const;
  void functionDeriv(const FunctionDomain& domain, Jacobian& jacobian);

  /// Function you want to fit.
  virtual void function1D(double* out, const double* xValues, const size_t nData)const = 0;
  /// Derivatives of function with respect to active parameters
  virtual void functionDeriv1D(Jacobian* out, const double* xValues, const size_t nData);

protected:

  /// Static reference to the logger class
  static Kernel::Logger& g_log;

};

typedef boost::shared_ptr<IFunction1D> IFunction1D_sptr;

} // namespace Numeric

#endif /*NUMERIC_IFUNCTION1D_H_*/
