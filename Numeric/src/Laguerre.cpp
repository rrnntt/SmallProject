#include "Numeric/Laguerre.h"

namespace Numeric
{

DECLARE_FUNCTION(Laguerre);

/// Constructor
Laguerre::Laguerre()
{
  declareParameter("Alpha",1.0,"The alpha parameter of the generalized Laguerre polynomial.");
  declareParameter("Height",1.0,"The function scaling factor.");
  declareParameter("Scaling",1.0,"The argument scaling factor.");
}

/// Function you want to fit to.
void Laguerre::function1D(double* out, const double* xValues, const size_t nData)const
{
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

/// Returns the value of the un-scaled generalized Laguerre polynomial
double Laguerre::unscaledLaguerre(double alpha, int n, double x)
{
}


} // Numeric
