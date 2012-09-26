#include "Numeric/Laguerre.h"
#include "Numeric/FunctionFactory.h"

namespace Numeric
{

DECLARE_FUNCTION(Laguerre);

/// Constructor
Laguerre::Laguerre()
{
  declareParameter("Alpha",0.0,"The alpha parameter of the generalized Laguerre polynomial.");
  declareParameter("Height",1.0,"The function scaling factor.");
  declareParameter("Scaling",1.0,"The argument scaling factor.");
}

/// Function you want to fit to.
void Laguerre::function1D(double* out, const double* xValues, const size_t nData)const
{
  const double alpha = getParameter("Alpha");
  if ( nData == 0 ) return;
  for(size_t i = 0; i < nData; ++i)
  {
    out[i] = unscaledLaguerre(alpha,m_n,xValues[i]);
  }
}

/// Derivatives of function with respect to active parameters
void Laguerre::functionDeriv(const FunctionDomain& domain, Jacobian& jacobian)
{
  calNumericalDeriv(domain,jacobian);
}

/// Return a value of attribute attName
Laguerre::Attribute Laguerre::getAttribute(const std::string& attName)const
{
  if (attName == "n")
  {
    return Attribute(m_n);
  }
  throw std::runtime_error("Attribute " + attName + " is undefined");
}

/**
 * Set a value to attribute attName
 * @param attName :: Attribute name, must be "Formula"
 * @param att :: Attribute value, must be a math expression, "x" is the function's argument
 */
void Laguerre::setAttribute(const std::string& attName,const Laguerre::Attribute& att)
{
  if (attName == "n")
  {
    m_n = att.asInt();
  }
  else
  {
    throw std::runtime_error("Attribute " + attName + " is undefined");
  }
}

/// Returns a list of attribute names
std::vector<std::string> Laguerre::getAttributeNames()const
{
  std::vector<std::string> out;
  out.push_back("n");
  return out;
}

/// Check if attribute attName exists
bool Laguerre::hasAttribute(const std::string& attName)const 
{
  return attName == "n";
}

/// Returns the value of the un-scaled generalized Laguerre polynomial
double Laguerre::unscaledLaguerre(double alpha, int n, double x)
{
  double p0 = 1.0;
  if ( n == 0 ) return p0;
  double p1 = -x + alpha + 1.0;
  if ( n == 1 ) return p1;
  double p;
  for(int k = 2; k <= n; ++k)
  {
    p = (2.0 + (alpha - 1.0 - x) / k) * p1 - (1.0 + (alpha - 1.0) / k) * p0;
    p0 = p1;
    p1 = p;
  }
  return p;
}


} // Numeric
