#include "QtAPI/FunctionCurve.h"

#include "Numeric/IFunction1D.h"

#include "qwt_scale_map.h"

#include <QPainter>

namespace QtAPI
{

/**
 * Constructor.
 */
FunctionCurve::FunctionCurve():
m_x(new Numeric::FunctionDomain1DVector(0.0))
{
  m_y.reset(*m_x);
}

/**
 * Destructor.
 */
FunctionCurve::~FunctionCurve()
{
}

/**
 * Set the curve values from two std vectors. Throws invalid_argument if x and y have
 * different sizes or x values are not in ascending order.
 * @param x :: The x values
 * @param y :: The y values
 */
void FunctionCurve::setData(const std::vector<double>& x, const std::vector<double>& y)
{
  if ( x.size() != y.size() )
  {
    throw std::invalid_argument("FunctionCurve: trying to set values from vectors of different sizes");
  }
  for(size_t i = 1; i < x.size(); ++i)
  {
    if ( x[i] <= x[i - 1] )
    {
      throw std::invalid_argument("FunctionCurve: x values must be in ascending order");
    }
  }
  m_x.reset(new Numeric::FunctionDomain1DVector(x));
  m_y.reset(*m_x);
  m_y.setCalculated(y);
  calcBoundingRect();
}

/**
 * Set the curve values from IFunction1D
 * @param domain :: Domain to set the x values
 * @param fun :: A IFunction which must be a IFunction1D
 */
void FunctionCurve::setData(const Numeric::FunctionDomain& domain, const Numeric::IFunction& fun)
{
  const Numeric::FunctionDomain1D* d1d = dynamic_cast<const Numeric::FunctionDomain1D*>(&domain);
  if ( !d1d )
  {
    throw std::invalid_argument("FunctionCurve: domain does not cast to FunctionDomain1D");
  }
  const Numeric::IFunction1D* f1d = dynamic_cast<const Numeric::IFunction1D*>(&fun);
  if ( !f1d )
  {
    throw std::invalid_argument("FunctionCurve: function does not cast to IFunction1D");
  }
  m_x.reset(new Numeric::FunctionDomain1DVector(*d1d));
  m_y.reset(*m_x);
  fun.function(domain,m_y);
  calcBoundingRect();
}

/// calculate and cache the current bounfing rect
void FunctionCurve::calcBoundingRect()
{
  const size_t n = m_x->size();
  if ( n == 0 )
  {
    m_boundingRect = QRectF();
  }
  double minX = (*m_x)[0];
  double maxX = (*m_x)[n - 1];
  double minY = m_y.getCalculated(0);
  double maxY = m_y.getCalculated(0);
  for(size_t i = 0; i < n; ++i)
  {
    const double y = m_y.getCalculated(i);
    if ( y < minY ) 
    {
      minY = y;
    }
    if ( y > maxY ) 
    {
      maxY = y;
    }
  }
  m_boundingRect = QRectF(minX,minY,maxX - minX, maxY - minY);
}

/// Return the bounding rectangle
QRectF FunctionCurve::boundingRect() const
{
  return m_boundingRect;
}


/**
 * Draws the curve efficiently.
 * @param painter Painter
 * @param xMap Maps x-values into pixel coordinates.
 * @param yMap Maps y-values into pixel coordinates.
 * @param canvasRect Contents rect of the canvas in painter coordinates
 */
void FunctionCurve::drawObject(QPainter *painter, 
  const QwtScaleMap &xMap, const QwtScaleMap &yMap,
  const QRect &canvasRect) const
{
  const double start = xMap.s1();
  const double end = xMap.s2();

  const size_t n = m_x->size();
  // find first point to draw which is 0 or the one before first visible
  size_t istart = 0;
  for(; istart < n; ++istart)
  {
    const double& x = (*m_x)[istart];
    if ( x < start ) continue;
    if ( x > end ) break;
    if ( istart > 0 ) --istart;
    break;
  }
  if ( istart >= n ) return;
  // draw the points
  int x1 = xMap.transform((*m_x)[istart]);
  int y1 = yMap.transform(m_y.getCalculated(istart));
  for(size_t i = istart + 1; i < n; ++i)
  {
    const double& x = (*m_x)[i];
    if ( x > end ) break;
    int x2 = xMap.transform(x);
    int y2 = yMap.transform(m_y.getCalculated(i));
    painter->drawLine(x1,y1,x2,y2);
    x1 = x2;
    y1 = y2;
  }
}



} // QtAPI
