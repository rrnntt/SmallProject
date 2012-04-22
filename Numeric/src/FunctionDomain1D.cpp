//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Numeric/FunctionDomain1D.h"
#include <iostream> 

namespace Numeric
{

/**
  * Create a domain form a vector.
  * @param xvalues :: Points
  */
FunctionDomain1DVector::FunctionDomain1DVector(const std::vector<double>& xvalues):
FunctionDomain1D(NULL,0)
{
  if (xvalues.empty())
  {
    throw std::invalid_argument("FunctionDomain1D cannot have zero size.");
  }
  m_X.assign(xvalues.begin(),xvalues.end());
  resetData(&m_X[0],m_X.size());
}

FunctionDomain1DVector::FunctionDomain1DVector(std::vector<double>::const_iterator from, std::vector<double>::const_iterator to):
FunctionDomain1D(NULL,0)
{
  if (from == to)
  {
    throw std::invalid_argument("FunctionDomain1D cannot have zero size.");
  }
  m_X.assign(from,to);
  resetData(&m_X[0],m_X.size());
}

FunctionDomain1DVector::FunctionDomain1DVector(const double startX, const double endX, const size_t n):
FunctionDomain1D(NULL,0)
{
  if (n == 0)
  {
    throw std::invalid_argument("FunctionDomain1D cannot have zero size.");
  }
  m_X.resize(n);
  if (n == 1)
  {
    m_X[0] = (startX + endX) / 2;
  }
  else
  {
    const double dx = (endX - startX) / (n - 1);
    for(size_t i = 0; i < n; ++i)
    {
      m_X[i] = startX + dx * i;
    }
  }
  resetData(&m_X[0],m_X.size());
}

FunctionDomain1DVector::FunctionDomain1DVector(const double x):
FunctionDomain1D(NULL,0)
{
  m_X.resize(1);
  m_X[0] = x;
  resetData(&m_X[0],m_X.size());
}

/**
 * Copy constructor.
 * @param right :: The other domain.
 */
FunctionDomain1DVector::FunctionDomain1DVector(const FunctionDomain1D& right):
FunctionDomain1D(NULL,0)
{
  *this = right;
}

/**
 * Copy constructor.
 * @param right :: The other domain.
 */
FunctionDomain1DVector::FunctionDomain1DVector(const FunctionDomain1DVector& right):
FunctionDomain1D(NULL,0)
{
  *this = right;
}

/**
 * Copy assignment operator.
 * @param right :: The other domain.
 */
FunctionDomain1DVector& FunctionDomain1DVector::operator=(const FunctionDomain1DVector& right)
{
  if (right.m_X.empty())
  {
    throw std::invalid_argument("FunctionDomain1D cannot have zero size.");
  }
  m_X.assign(right.m_X.begin(),right.m_X.end());
  resetData(&m_X[0],m_X.size());
  return *this;
}

/**
 * Copy assignment operator.
 * @param right :: The other domain.
 */
FunctionDomain1DVector& FunctionDomain1DVector::operator=(const FunctionDomain1D& right)
{
  if (right.size() == 0)
  {
    throw std::invalid_argument("FunctionDomain1D cannot have zero size.");
  }
  const double* data = right.getPointerAt(0);
  m_X.assign(data, data + right.size());
  resetData(&m_X[0],m_X.size());
  return *this;
}

} // namespace Numeric
