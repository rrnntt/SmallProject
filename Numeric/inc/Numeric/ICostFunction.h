#ifndef NUMERIC_ICOSTFUNCTION_H_
#define NUMERIC_ICOSTFUNCTION_H_

//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Numeric/DllExport.h"
#include "Numeric/CostFunctionFactory.h"

#include <boost/shared_ptr.hpp>

namespace Numeric
{
/** An interface for specifying the cost function to be used with Fit,
    for example, the default being least squares fitting
*/
class NUMERIC_EXPORT ICostFunction 
{
public:
  /// Virtual destructor
  virtual ~ICostFunction() {}

  /// Get name of minimizer
  virtual std::string name() const = 0;

  /// Get short name of minimizer - useful for say labels in guis
  virtual std::string shortName() const {return "Quality";}

  /// Get i-th parameter
  /// @param i :: Index of a parameter
  /// @return :: Value of the parameter
  virtual double getParameter(size_t i)const = 0;
  /// Set i-th parameter
  /// @param i :: Index of a parameter
  /// @param value :: New value of the parameter
  virtual void setParameter(size_t i, const double& value) = 0;
  /// Number of parameters
  virtual size_t nParams()const = 0;

  /// Calculate value of cost function
  virtual double val() const = 0;

  /// Calculate the derivatives of the cost function
  /// @param der :: Container to output the derivatives
  virtual void deriv(std::vector<double>& der) const = 0;

  /// Calculate the value and the derivatives of the cost function
  /// @param der :: Container to output the derivatives
  /// @return :: The value of the function
  virtual double valAndDeriv(std::vector<double>& der) const = 0;
};

/// define a shared pointer to a cost function
typedef boost::shared_ptr<ICostFunction> ICostFunction_ptr;

/**
 * Macro for declaring a new type of cost functions to be used with the CostFunctionFactory
 */
#define DECLARE_COSTFUNCTION(classname,username) \
        namespace { \
	int register_costfunction_##classname( \
  ((Numeric::CostFunctionFactory::instance().subscribe<classname>(#username)) \
	, 0)); \
	} 

} // namespace Numeric

#endif /*NUMERIC_ICOSTFUNCTION_H_*/
