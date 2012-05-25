#include "Numeric/UserFunction1D.h"
#include "Numeric/FunctionFactory.h"
#include "Formula/Namespace.h"
#include "Formula/Expression.h"
#include "Formula/Scalar.h"

namespace Numeric
{

DECLARE_FUNCTION(UserFunction1D);

/// Function you want to fit to.
void UserFunction1D::function1D(double* out, const double* xValues, const size_t nData)const
{
  if (!m_namespace || !m_expression)
  {
    throw std::runtime_error("UserFunction1D is not set");
  }
  for(size_t i = 0; i < nData; ++i)
  {
    m_x = xValues[i];
    out[i] = m_expression->eval().as<Formula::Scalar>();
  }
}

/// Return a value of attribute attName
UserFunction1D::Attribute UserFunction1D::getAttribute(const std::string& attName)const
{
  if (attName == "Formula")
  {
    return Attribute(m_formula);
  }
  throw std::runtime_error("Attribute " + attName + " is undefined");
}

/**
 * Set a value to attribute attName
 * @param attName :: Attribute name, must be "Formula"
 * @param att :: Attribute value, must be a math expression, "x" is the function's argument
 */
void UserFunction1D::setAttribute(const std::string& attName,const UserFunction1D::Attribute& att)
{
  if (attName == "Formula")
  {
    m_formula = att.asString();
    setFormula();
  }
  else
  {
    throw std::runtime_error("Attribute " + attName + " is undefined");
  }
}

/**
 * Parse the formula, create the expression, and declare parameters
 */
void UserFunction1D::setFormula()
{
  m_namespace.reset(new Formula::Namespace);
  Kernel::EParser parser;
  parser.parse(m_formula);
  auto vars = parser.getVariables();
  Kernel::NumberParser num;
  //if (vars.count("x") == 0)
  //{
  //  throw std::invalid_argument("Formula must contain the x variable:\n"+m_formula);
  //}
  for(auto var = vars.begin(); var != vars.end(); ++var)
  {
    const std::string& varName = *var;
    if (varName == "x")
    {
      m_namespace->addVariable(Formula::Variable_ptr(new Formula::Scalar(&m_x)),varName);
    }
    else
    {
      num.match(varName);
      if (!num.hasMatch())
      {
        this->declareParameter(varName);
      }
    }
  }
  for(size_t i = 0; i < nParams(); ++i)
  {
    const std::string& varName = parameterName(i);
    m_namespace->addVariable(Formula::Variable_ptr(new Formula::Scalar(this->getParameterAddress(i))),varName);
  }
  m_expression.reset(new Formula::Expression(m_namespace,parser));

}

} // namespace Numeric
