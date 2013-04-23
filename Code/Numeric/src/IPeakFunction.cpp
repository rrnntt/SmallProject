//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Numeric/IPeakFunction.h"
#include "Numeric/Jacobian.h"

#include <boost/lexical_cast.hpp>
#include <cmath>

namespace Numeric
{

/** A Jacobian for individual functions
 */
class PartialJacobian1: public Jacobian
{
  Jacobian* m_J;  ///< pointer to the overall Jacobian
  int m_iY0;      ///< offset in the overall Jacobian for a particular function
public:
  /** Constructor
   * @param J :: A pointer to the overall Jacobian
   * @param iY0 :: The data offset for a particular function
   */
  PartialJacobian1(Jacobian* J,int iY0):m_J(J),m_iY0(iY0)
  {}
  /**
   * Overridden Jacobian::set(...).
   * @param iY :: The index of the data point
   * @param iP :: The parameter index of an individual function.
   * @param value :: The derivative value
   */
  void set(size_t iY, size_t iP, double value)
  {
      m_J->set(m_iY0 + iY,iP,value);
  }
  /**
   * Overridden Jacobian::get(...).
   * @param iY :: The index of the data point
   * @param iP :: The parameter index of an individual function.
   */
  double get(size_t iY, size_t iP)
  {
      return m_J->get(m_iY0 + iY,iP);
  }
};

/**
  * Constructor. Sets peak radius to the value of curvefitting.peakRadius property
  */
IPeakFunction::IPeakFunction():
m_peakRadius(5)
{
}

/** 
 * General implementation of the method for all peaks. Limits the peak evaluation to
 * a certain number of FWHMs around the peak centre. The outside points are set to 0. 
 * Calls functionLocal() to compute the actual values
 * @param out :: Output function values
 * @param xValues :: X values for data points
 * @param nData :: Number of data points
 */
void IPeakFunction::function1D(double* out, const double* xValues, const size_t nData)const
{
  double c = this->centre();
  double dx = fabs(m_peakRadius*this->width());
  int i0 = -1;
  int n = 0;
  for(size_t i = 0; i < nData; ++i)
  {
    if (fabs(xValues[i] - c) < dx)
    {
      if (i0 < 0) i0 = static_cast<int>(i);
      ++n;
    }
    else
    {
      out[i] = 0.0;
    }
  }
  if (i0 < 0 || n == 0) return;
  this->functionLocal(out+i0, xValues+i0, n);

}

/** 
 * General implementation of the method for all peaks. Calculates derivatives only
 * for a range of x values limited to a certain number of FWHM around the peak centre.
 * For the points outside the range all derivatives are set to 0.
 * Calls functionDerivLocal() to compute the actual values
 * @param out :: Derivatives
 * @param xValues :: X values for data points
 * @param nData :: Number of data points
 */
void IPeakFunction::functionDeriv1D(Jacobian* out, const double* xValues, const size_t nData)
{
  double c = this->centre();
  double dx = fabs(m_peakRadius*this->width());
  int i0 = -1;
  int n = 0;
  for(size_t i = 0; i < nData; ++i)
  {
    if (fabs(xValues[i] - c) < dx)
    {
      if (i0 < 0) i0 = static_cast<int>(i);
      ++n;
    }
    else
    {
      for(size_t ip = 0; ip < this->nParams(); ++ip)
      {
        out->set(i,ip, 0.0);
      }
    }
  }
  if (i0 < 0 || n == 0) return;
  PartialJacobian1 J(out,i0);
  this->functionDerivLocal(&J,xValues+i0,n);
}

void IPeakFunction::setPeakRadius(const int& r)
{
  if (r > 0)
  {
    m_peakRadius = r;
  }
}


} // namespace Numeric
