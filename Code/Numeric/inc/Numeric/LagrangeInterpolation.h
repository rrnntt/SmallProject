#ifndef NUMERIC_LAGRANGEINTERPOLATION_H
#define NUMERIC_LAGRANGEINTERPOLATION_H

#include "Numeric/DllExport.h"
#include "Numeric/IFunction1D.h"
#include "Numeric/ParamFunction.h"
#include "Numeric/GSLMatrix.h"

namespace Numeric
{

class NUMERIC_EXPORT LagrangeInterpolation: public IFunction1D, public ParamFunction
{
public:
  LagrangeInterpolation(const std::vector<double>& r);
  ~LagrangeInterpolation();
  /// Name of the function
  virtual std::string name()const {return "LagrangeInterpolation";}
  /// Function you want to fit to.
  void function1D(double* out, const double* xValues, const size_t nData)const;
  /// size: number of interpolation points
  size_t size() const {return m_r.size();}
  /// set a function value
  void setValue(size_t i, double v);
protected:
  /// initialize the instance.
  void init();

  /// interpolation points (roots)
  std::vector<double> m_r;
  /// function values at interpolation points
  std::vector<double> m_p;
  /// matrix of distances between the roots
  GSLMatrix m_D;
};

} // Numeric

#endif // NUMERIC_LAGRANGEINTERPOLATION_H
