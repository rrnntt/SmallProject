#ifndef NUMERIC_FUNCTIONDOMAIN1D_H
#define NUMERIC_FUNCTIONDOMAIN1D_H

#include "Numeric/DllExport.h"
#include "Numeric/FunctionDomain.h"

namespace Numeric
{

class NUMERIC_EXPORT FunctionDomain1D: public FunctionDomain
{
public:
  FunctionDomain1D(double start, double end, size_t n);
  FunctionDomain1D(const std::vector<double>& xvalues);
  /// get an x value
  /// @param i :: Index
  double getX(size_t i) const {return m_X.at(i);}
protected:
  std::vector<double> m_X; ///< vector of function arguments
};

} // namespace Numeric
#endif // NUMERIC_FUNCTIONDOMAIN1D_H
