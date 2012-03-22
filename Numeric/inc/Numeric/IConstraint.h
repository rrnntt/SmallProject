#ifndef NUMERIC_ICONSTRAINT_H_
#define NUMERIC_ICONSTRAINT_H_

//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Numeric/IFunction.h"
#include "Numeric/ParameterReference.h"

namespace Kernel
{
  class EParser;
}

namespace Numeric
{
/** 
 * An interface to a constraint.
 */
class NUMERIC_EXPORT IConstraint: public ParameterReference
{
public:
  /// Default constructor
  IConstraint() {}
  /// Virtual destructor
  virtual ~IConstraint() {}

  /// Initialize the constraint from an expression
  virtual void initialize(IFunction* fun, const Kernel::EParser& expr) = 0;

  /// Returns a penalty number which is bigger than or equal to zero
  /// If zero it means that the constraint is not penalized. If larger
  /// than zero the constraint is penalized where the larger this number
  /// is the larger the penalty
  virtual double check() = 0;

  /// Returns the derivative of the penalty for each active parameter
  virtual double checkDeriv() = 0;

  /// Returns the derivative of the penalty for each active parameter
  virtual double checkDeriv2() = 0;

  /// Set the parameters of IFitFunction to satisfy constraint. For example
  /// for a BoundaryConstraint this if param value less than lower boundary
  /// it is set to that value and vice versa for if the param value is larger
  /// than the upper boundary value. 
  virtual void setParamToSatisfyConstraint() = 0;

  /// set the penalty factor for the constraint
  /// Set panelty factor. The larger the number to thigter the constraint. This number
  /// must be set to a number larger than zero
  /// @param c :: the penalt factor
  virtual void setPenaltyFactor(const double& c) = 0;   

  /// get the penalty factor for the constraint
  virtual double getPenaltyFactor()const = 0;   

  /// Return the string that can be used in this->initialize() to recreate this constraint
  virtual std::string asString()const = 0;
};


} // namespace Numeric


#endif /*NUMERIC_ICONSTRAINT_H_*/
