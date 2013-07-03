#include "Numeric/FunctionDomainMD.h"

namespace Numeric
{

/**
 * Constructor.
 * @param dims :: Number of dimensions in this domain.
 */
FunctionDomainMDGrid::FunctionDomainMDGrid(size_t dims):
    m_grid(dims,std::vector<double>(1,0.0)),
    m_pointBuffer(dims,0.0)
{
}

/**
 * Get the point.
 * @param index :: Index of a point.
 * @return :: Array of point vector components.
 */
const double *FunctionDomainMDGrid::getPoint(size_t index) const
{
    for(size_t i = 0; i < m_grid.size() - 1; ++i)
    {
        size_t n = m_grid[i].size();
        size_t k = index % n;
        index /= n;
        m_pointBuffer[i] = m_grid[i][k];
    }
    m_pointBuffer.back() = m_grid.back()[index];
    return &m_pointBuffer[0];
}

/**
 * @return :: Size of the domain.
 */
size_t FunctionDomainMDGrid::size() const
{
    size_t n = 1;
    for(size_t i = 0; i < m_grid.size(); ++i)
    {
        n *= m_grid[i].size();
    }
    return n;
}

/**
 * Set the grid values along one dimension.
 *
 * @param i :: Dimension index to set.
 * @param axis :: Coordinates on the i-th axis.
 */
void FunctionDomainMDGrid::setDimension(size_t i, const std::vector<double> &axis)
{
    if ( i >= ndim() )
    {
        throw std::runtime_error("Dimension index is out of range.");
    }
    m_grid[i].assign(axis.begin(),axis.end());
}

} // Numeric
