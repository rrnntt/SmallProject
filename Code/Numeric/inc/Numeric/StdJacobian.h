#ifndef NUMERIC_STDJACOBIAN_H
#define NUMERIC_STDJACOBIAN_H

#include "Numeric/DllExport.h"
#include "Numeric/Jacobian.h"

#include <vector>
#include <stdexcept>

namespace Numeric
{

class NUMERIC_EXPORT StdJacobian: public Jacobian
{
  /// Number of data points
  size_t m_ny;
  /// Number of parameters in a function (== IFunction::nParams())
  size_t m_np;
  /// Storage for the derivatives
  std::vector<double> m_data;
public:
  /// Constructor.
  /// @param ny :: Number of data points
  /// @param np :: Number of parameters
  StdJacobian(size_t ny,size_t np): m_ny(ny), m_np(np)
  {
    m_data.resize(ny * np, 0.0);
  }
  /// overwrite base method
  /// @param value :: the value
  /// @param iP :: the index of the parameter
  ///  @throw runtime_error Thrown if column of Jacobian to add number to does not exist
  void addNumberToColumn(const double& value, const size_t& iP)
  {
    if (iP < m_np )
    {
      // add penalty to first and last point and every 10th point in between
      m_data[iP] += value;
      m_data[(m_ny - 1) * m_np + iP] += value;
      for (size_t iY = 9; iY < m_ny; iY++) 
        m_data[iY*m_np + iP] += value;
    }
    else
    {
      throw std::runtime_error("Try to add number to column of Jacobian matrix which does not exist.");
    }   
  }
  /// overwrite base method
  void set(size_t iY, size_t iP, double value)
  {
    if (iY >= m_ny)
    {
      throw std::out_of_range("Data index in Jacobian is out of range");
    }
    if (iP >= m_np)
    {
      throw std::out_of_range("Parameter index in Jacobian is out of range");
    }
    m_data[iY*m_np + iP] = value;
  }
  /// overwrite base method
  double get(size_t iY, size_t iP)
  {
    if (iY >= m_ny)
    {
      throw std::out_of_range("Data index in Jacobian is out of range");
    }
    if (iP >= m_np)
    {
      throw std::out_of_range("Parameter index in Jacobian is out of range");
    }
    return m_data[iY*m_np + iP];
  }
};


} // NUMERIC

#endif // NUMERIC_STDJACOBIAN_H
