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
FunctionDomain1D::FunctionDomain1D(const std::vector<double>& xvalues)
{
  if (xvalues.empty())
  {
    throw std::invalid_argument("FunctionDomain1D cannot have zero size.");
  }
  m_X.assign(xvalues.begin(),xvalues.end());
}

FunctionDomain1D::FunctionDomain1D(std::vector<double>::const_iterator from, std::vector<double>::const_iterator to)
{
  if (from == to)
  {
    throw std::invalid_argument("FunctionDomain1D cannot have zero size.");
  }
  m_X.assign(from,to);
}

FunctionDomain1D::FunctionDomain1D(const double startX, const double endX, const size_t n)
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
}

FunctionDomain1D::FunctionDomain1D(const double x)
{
  m_X.resize(1);
  m_X[0] = x;
}

} // namespace Numeric
