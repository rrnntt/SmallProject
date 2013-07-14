#ifndef NUMERIC_TIESFUNCTION_H
#define NUMERIC_TIESFUNCTION_H

#include "Numeric/DllExport.h"
#include "Numeric/IFunction.h"

namespace Numeric
{
//----------------------------------------------------------------------
// Forward declarations
//----------------------------------------------------------------------
class Jacobian;
class ParameterTie;
class IConstraint;

/**
 * Implements virtual methods of IFunction wich deal with ties and constarints.
 */
class NUMERIC_EXPORT TiesFunction: public virtual IFunction
{
public:
  ~TiesFunction();

  /// Apply the ties
  virtual void applyTies();
  /// Remove all ties
  virtual void clearTies();
  virtual void removeTie(const std::string& parName){IFunction::removeTie(parName);}
  /// Removes i-th parameter's tie
  virtual bool removeTie(size_t i);
  /// Get the tie of i-th parameter
  virtual ParameterTie* getTie(size_t i)const;

  /// Add a constraint to function
  virtual void addConstraint(IConstraint* ic);
  /// Get constraint of i-th parameter
  virtual IConstraint* getConstraint(size_t i)const;
  /// Remove a constraint
  virtual void removeConstraint(const std::string& parName);
  /// Set parameters to satisfy constraints
  void setParametersToSatisfyConstraints();

  /// Return parameter index from a parameter reference. Usefull for constraints and ties in composite functions
  virtual size_t getParameterIndex(const ParameterReference& ref)const;

protected:
  /// Add a new tie
  virtual void addTie(ParameterTie* tie);

private:
  /// Holds parameter ties as <parameter index,tie pointer> 
  std::vector<ParameterTie*> m_ties;
  /// Holds the constraints added to function
  std::vector<IConstraint*> m_constraints;
};

} // Numeric

#endif // NUMERIC_TIESFUNCTION_H
