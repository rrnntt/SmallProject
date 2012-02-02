#include "Numeric/FunctionDomain1D.h"

namespace Numeric
{

  /**
   * Create a domain from interval bounds and number of divisions.
   * @param start :: Starting point
   * @param end :: End point
   * @param n :: Number of points
   */
  FunctionDomain1D::FunctionDomain1D(double start, double end, size_t n):
  FunctionDomain(n)
  {
    // n > 0, FunctionDomain(n) must throw if it isn't
    double dx = (end - start) / (static_cast<double>(n) - 1.0);
    m_X.resize(n);
    double x = start;
    for(size_t i = 0; i < n; ++i, x += dx)
    {
      m_X[i] = x;
    }
  }

  /**
   * Create a domain form a vector.
   * @param xvalues :: Points
   */
  FunctionDomain1D::FunctionDomain1D(const std::vector<double>& xvalues):
  FunctionDomain(xvalues.size())
  {
    m_X.assign(xvalues.begin(),xvalues.end());
  }

} // namespace Numeric
