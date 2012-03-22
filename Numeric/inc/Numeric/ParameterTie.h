#ifndef NUMERIC_PARAMETERTIE_H_
#define NUMERIC_PARAMETERTIE_H_

//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Numeric/DllExport.h"
#include "Numeric/IFunction.h"
#include "Numeric/ParameterReference.h"

#include <map>

namespace Numeric
{
/** Ties fitting parameters. A tie is a formula that is used to 
    calculate the value of a function parameter based on the values of other parameters.
    A tied parameter is not considered independent and doesn't take part in fitting.
    Its value is always calculated with its tie's formula.

  */
class NUMERIC_EXPORT ParameterTie: public ParameterReference
{
public:
  /// Constructor
  ParameterTie(IFunction* funct,const std::string& parName,const std::string& expr = "");
  /// Destructor
  virtual ~ParameterTie();
  /// Set the tie expression
  virtual void set(const std::string& expr);
  /// Evaluate the expression
  virtual double eval();
  /// Return the string that can be used to recreate this tie
  virtual std::string asString(const IFunction* fun = NULL)const;

  /// Check if the tie has any references to certain parameters
  bool findParametersOf(const IFunction* fun)const;

protected:
  //mu::Parser* m_parser;         ///< math parser
  /// Store for parameters used in the tie. The map's key is address used bu the mu::Parser
  std::map<double*,ParameterReference> m_varMap;
  /// Keep the function that was passed to the constructor
  IFunction* m_function1;
  /// Keep the template for the input string passed to this->set(...)
  /// In the template CompositeFunction prefixes are replaced with placeholders
  std::string m_expression;

private:
  /// MuParser callback function
  static double* AddVariable(const char *varName, void *palg);
};

} // namespace Numeric

#endif /*MANTID_API_PARAMETERTIE_H_*/
