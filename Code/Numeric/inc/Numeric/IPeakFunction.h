#ifndef NUMERIC_IPEAKFUNCTION_H_
#define NUMERIC_IPEAKFUNCTION_H_

//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Numeric/IFunction1D.h"

namespace Numeric
{
/** An interface to a peak function, which extend the interface of 
    IFunctionWithLocation by adding methods to set and get peak width.
*/
class NUMERIC_EXPORT IPeakFunction : public IFunction1D
{
public:
  /// Constructor
  IPeakFunction();

  /// Returns the peak FWHM
  virtual double width()const = 0;
  /// Sets the parameters such that FWHM = w
  virtual void setWidth(const double w) = 0;

  /// Returns the peak max height
  virtual double height()const = 0;
  /// Sets the parameters such that height == h
  virtual void setHeight(const double h) = 0;

  /// Returns the peak centre
  virtual double centre()const = 0;
  /// Sets the parameters such that centre == c
  virtual void setCentre(const double c) = 0;

  /// General implementation of the method for all peaks. 
  void function1D(double* out, const double* xValues, const size_t nData)const;
  /// General implementation of the method for all peaks. 
  void functionDeriv1D(Jacobian* out, const double* xValues, const size_t nData);
  /// Set new peak radius
  void setPeakRadius(const int& r = 5);

protected:
  /// Function evaluation method to be implemented in the inherited classes
  virtual void functionLocal(double* out, const double* xValues, const size_t nData)const = 0;
  /// Derivative evaluation method to be implemented in the inherited classes
  virtual void functionDerivLocal(Jacobian* out, const double* xValues, const size_t nData) = 0;
  /// Defines the area around the centre where the peak values are to be calculated (in FWHM).
  int m_peakRadius; 
};

} // namespace Numeric

#endif /*MANTID_API_IPEAKFUNCTION_H_*/
