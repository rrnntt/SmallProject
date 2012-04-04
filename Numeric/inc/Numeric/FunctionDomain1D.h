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
  /// Return the number of arguments in the domain
  virtual size_t size() const  {return m_n;}
  /// Get an x value.
  /// @param i :: Index
  double operator[](size_t i) const {return m_data[i];}
  /// Get a pointer to i-th value
  const double* getPointerAt(size_t i) const {return m_data + i;}
protected:
  /// Protected constructor, shouldn't be created directly. Use FunctionDomain1DView instead.
  FunctionDomain1D(const double* x, size_t n):m_data(x),m_n(n){}
  /// Reset the pointer and size of the domain
  void resetData(const double* x, size_t n)
  {
    m_data = x;
    m_n = n;
  }
private:
  FunctionDomain1D(const FunctionDomain1D& right);
  FunctionDomain1D& operator=(const FunctionDomain1D&);
  const double *m_data;///< pointer to the start of the domain data
  size_t m_n; ///< size of the data
};

class NUMERIC_EXPORT FunctionDomain1DVector: public FunctionDomain1D
{
public:
  FunctionDomain1DVector(const double x);
  FunctionDomain1DVector(const double startX, const double endX, const size_t n);
  FunctionDomain1DVector(const std::vector<double>& xvalues);
  FunctionDomain1DVector(std::vector<double>::const_iterator from, std::vector<double>::const_iterator to);
  FunctionDomain1DVector(const FunctionDomain1DVector&);
  FunctionDomain1DVector& operator=(const FunctionDomain1DVector&);
protected:
  std::vector<double> m_X; ///< vector of function arguments
};

/**
 * 1D domain - a wrapper around an array of doubles. 
 */
class NUMERIC_EXPORT FunctionDomain1DView: public FunctionDomain1D
{
public:
  FunctionDomain1DView(const double* x, size_t n):FunctionDomain1D(x,n){}
private:
  FunctionDomain1DView(const FunctionDomain1DView&);
  FunctionDomain1DView& operator=(const FunctionDomain1DView&);
};

/// typedef for a shared pointer to a FunctionDomain1D
typedef boost::shared_ptr<FunctionDomain1D> FunctionDomain1D_ptr;
typedef boost::shared_ptr<const FunctionDomain1D> FunctionDomain1D_const_ptr;

} // namespace Numeric

#endif /*NUMERIC_FUNCTIONDOMAIN1D_H_*/
