#ifndef NUMERIC_PARAMETERREFERENCE_H_
#define NUMERIC_PARAMETERREFERENCE_H_

//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Numeric/DllExport.h"
#include "Numeric/IFunction.h"

namespace Numeric
{
/** 
    A reference to a parameter in a function. To uniquely identify a parameter
    in a composite function
  */
class NUMERIC_EXPORT ParameterReference
{
public:
  ParameterReference();
  ParameterReference(IFunction* fun, std::size_t index);
  std::size_t getIndex() const;
  void reset(IFunction* fun, std::size_t index);
  void setParameter(const double& value);
  double getParameter() const;
  IFunction* getFunction() const;
private:
  IFunction* m_function; ///< pointer to the function
  std::size_t m_index; ///< parameter index
};

} // namespace Numeric

#endif /*NUMERIC_PARAMETERREFERENCE_H_*/
