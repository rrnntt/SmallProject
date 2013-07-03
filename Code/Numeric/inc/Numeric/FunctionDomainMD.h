#ifndef NUMERIC_FUNCTIONDOMAINMD_H
#define NUMERIC_FUNCTIONDOMAINMD_H

#include "Numeric/DllExport.h"
#include "Numeric/FunctionDomain.h"

#include <vector>

namespace Numeric
{

/**
 * @brief The FunctionDomainMD class is an interface to a multi-dimensional domain.
 *
 */
class NUMERIC_EXPORT FunctionDomainMD: public FunctionDomain
{
public:
  /// Dimesionality of the domain ( == number of real values per point)
  virtual size_t ndim() const = 0;
  /// Get pointer to a buffer ndim() doubles long containing i-th point of the domain.
  /// Implementation must guarantee that the buffer remains valid (allocated) during lifetime
  /// of the class instance and that the values in it don't change until next call to getPoint(i).
  virtual const double *getPoint(size_t i) const = 0;
};

class NUMERIC_EXPORT FunctionDomainMDGrid: public FunctionDomainMD
{
public:
    FunctionDomainMDGrid(size_t dims);
    size_t ndim() const {return m_pointBuffer.size();}
    const double *getPoint(size_t index) const;
    size_t size() const;
    void setDimension(size_t i, const std::vector<double>& axis);
private:
    /// Grid divisions for each dimension
    std::vector<std::vector<double>> m_grid;
    /// Buffer for values of the current point (returned by getPoint(i))
    mutable std::vector<double> m_pointBuffer;
};

} // Numeric

#endif // NUMERIC_FUNCTIONDOMAINMD_H
