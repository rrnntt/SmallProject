#include "Numeric/UserFunction1D.h"
#include "Formula/Namespace.h"
#include "Formula/Expression.h"

namespace Numeric
{

/// Function you want to fit to.
void UserFunction1D::function1D(double* out, const double* xValues, const size_t nData)const
{
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
  }
  throw std::runtime_error("Attribute " + attName + " is undefined");
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
  if (vars.count("x") == 0)
  {
    throw std::invalid_argument("Formula must contain the x variable");
  }
  for(auto var = vars.begin(); var != vars.end(); ++var)
  {
    const std::string& varName = *var;
    m_namespace->addVariable("Scalar",varName);
    if (varName != "x")
    {
      this->declareParameter(varName);
    }
  }
  m_expression.reset(new Formula::Expression(m_namespace,parser));

}

} // namespace Numeric
