//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Numeric/FunctionDomain1D.h"
#include <iostream> 

namespace Numeric
{

/**
 * Create a sub-domain with point in a given interval.
 * @param start :: Start of the interval.
 * @param end :: End of the interval.
 */
boost::shared_ptr<FunctionDomain1D> FunctionDomain1D::createSubDomain(double start, double end) const
{
  if ( m_n < 2 ) return FunctionDomain1D_sptr();
  const double lowerBound = m_data[0];
  const double upperBound = m_data[m_n - 1];
  if ( start > upperBound || end < lowerBound ) return FunctionDomain1D_sptr();
  std::vector<double> vec;
  for(size_t i = 0; i < m_n; ++i)
  {
    const double& x = m_data[i];
    if ( x < start ) continue;
    if ( x > end ) break;
    vec.push_back( x );
  }

  if ( vec.empty() ) return FunctionDomain1D_sptr();

  return FunctionDomain1D_sptr(new FunctionDomain1DVector( vec ));
}


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
