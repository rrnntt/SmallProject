#ifndef NUMERIC_CHEBFUN2DSPLINE_H
#define NUMERIC_CHEBFUN2DSPLINE_H

#include "Numeric/DllExport.h"
#include "Numeric/Chebfun.h"

typedef double (*AFunction2D)(double,double);

namespace Numeric
{

class NUMERIC_EXPORT Chebfun2DSpline
{
public:
  Chebfun2DSpline(
    size_t nx, double startX, double endX, size_t nXLines,
    size_t ny, double startY, double endY, size_t nYLines
    );
  ~Chebfun2DSpline();
  /// Order of the x-functions
  size_t nx() const {return m_xBase->n;}
  /// Order of the y-functions
  size_t ny() const {return m_yBase->n;}
  /// Number of grid lines with constant x ( == number of y-functions )
  size_t nXLines() const {return m_yLines.size();}
  /// Number of grid lines with constant y ( == number of x-functions )
  size_t nYLines() const {return m_yLines.size();}
  /// Get the base of the x-functions
  ChebfunBase_sptr getXBase() {return m_xBase;}
  /// Get the base of the y-functions
  ChebfunBase_sptr getYBase() {return m_yBase;}
  /// evaluate the function
  double operator()(const double& x, const double& y)const;

  void fit( AFunction2D fun );
protected:
  /// functions of x at constant y's
  std::vector< chebfun > m_xFuns;
  /// functions of y at constant x's
  std::vector< chebfun > m_yFuns;
  /// Lines of constant x
  std::vector<double> m_xLines;
  /// Lines of constant y
  std::vector<double> m_yLines;
  ChebfunBase_sptr m_xBase;
  ChebfunBase_sptr m_yBase;
  mutable chebfun m_xLineFun;
  mutable chebfun m_yLineFun;
};

} // NUMERIC

#endif // NUMERIC_CHEBFUN2DSPLINE_H
