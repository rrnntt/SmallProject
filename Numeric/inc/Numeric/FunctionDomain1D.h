#ifndef NUMERIC_FUNCTIONDOMAIN1D_H_
#define NUMERIC_FUNCTIONDOMAIN1D_H_

//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Numeric/FunctionDomain.h"

#include <vector>

namespace Numeric
{
/** Base class that represents the domain of a function.
    A domain is a generalisation of x (argument) and y (value) arrays.
    A domain consists at least of a list of function arguments for which a function should 
    be evaluated and a buffer for the calculated values. If used in fitting also contains
    the fit data and weights.

*/
class NUMERIC_EXPORT FunctionDomain1D: public FunctionDomain
{
public:
  FunctionDomain1D(const double x);
  FunctionDomain1D(const double startX, const double endX, const size_t n);
  FunctionDomain1D(const std::vector<double>& xvalues);
  FunctionDomain1D(std::vector<double>::const_iterator from, std::vector<double>::const_iterator to);
  /// Return the number of arguments in the domain
  virtual size_t size() const  {return m_X.size();}
  /// Get an x value.
  /// @param i :: Index
  double operator[](size_t i) const {return m_X.at(i);}
  /// Get a pointer to i-th value
  const double* getPointerAt(size_t i) const {return &m_X.at(i);}
protected:
  std::vector<double> m_X; ///< vector of function arguments
};

/// typedef for a shared pointer to a FunctionDomain1D
typedef boost::shared_ptr<FunctionDomain1D> FunctionDomain1D_ptr;
typedef boost::shared_ptr<const FunctionDomain1D> FunctionDomain1D_const_ptr;

} // namespace Numeric

#endif /*NUMERIC_FUNCTIONDOMAIN1D_H_*/
