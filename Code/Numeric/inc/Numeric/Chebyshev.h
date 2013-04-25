#ifndef NUMERIC_CHEBYSHEV_H
#define NUMERIC_CHEBYSHEV_H

#include "Numeric/DllExport.h"
#include "Numeric/IFunction1D.h"
#include "Numeric/ParamFunction.h"

#include <valarray>

namespace Numeric
{

class NUMERIC_EXPORT Chebyshev: public IFunction1D, public ParamFunction
{
public:
  /// Constructor
  Chebyshev();
  /// Destructor
  ~Chebyshev() {};

  /// overwrite IFunction base class methods
  std::string name()const{return "Chebyshev";}
  void function1D(double* out, const double* xValues, const size_t nData)const;
  void functionDeriv1D(Jacobian* out, const double* xValues, const size_t nData);

  /// Returns the number of attributes associated with the function
  size_t nAttributes()const{return 3;}
  /// Returns a list of attribute names
  std::vector<std::string> getAttributeNames()const;
  /// Return a value of attribute attName
  Attribute getAttribute(const std::string& attName)const;
  /// Set a value to attribute attName
  void setAttribute(const std::string& attName,const Attribute& );
  /// Check if attribute attName exists
  bool hasAttribute(const std::string& attName)const;

private:

  /// Polynomial order
  int m_n;
  /// Lower x boundary. The default is -1
  double m_StartX;
  /// Upper x boundary. The default is  1
  double m_EndX;
  /// Keep intermediate calculatons
  mutable std::valarray<double> m_b;

};

} // NUMERIC

#endif // NUMERIC_CHEBYSHEV_H
