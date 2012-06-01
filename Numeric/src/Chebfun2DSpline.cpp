#include "Numeric/Chebfun2DSpline.h"

#include <algorithm>

namespace Numeric
{

/**
 * Constructor.
 */
Chebfun2DSpline::Chebfun2DSpline(
    size_t nx, double startX, double endX, size_t nXLines,
    size_t ny, double startY, double endY, size_t nYLines
  ):
m_xBase(new ChebfunBase(nx, startX, endX)),
m_yBase(new ChebfunBase(ny, startY, endY)),
m_xLineFun(nXLines-1,startX,endX),
m_yLineFun(nYLines-1,startY,endY)
{
  m_xFuns.resize( nYLines );
  m_yLines.resize( nYLines );
  const double dy = (endY - startY) / (nYLines - 1);
  for(size_t i = 0; i < m_xFuns.size(); ++i)
  {
    m_yLines[i] = startY + i * dy;
    m_xFuns[i].setBase( m_xBase );
  }

  m_yFuns.resize( nXLines );
  m_xLines.resize( nXLines );
  const double dx = (endX - startX) / (nXLines - 1);
  for(size_t i = 0; i < m_yFuns.size(); ++i)
  {
    m_xLines[i] = startX + i * dx;
    m_yFuns[i].setBase( m_yBase );
  }
}

/**
 * Destructor.
 */
Chebfun2DSpline::~Chebfun2DSpline()
{
}

/**
 * Fit to a function.
 * @param fun :: A function.
 */
void Chebfun2DSpline::fit( AFunction2D fun )
{
  auto& xv = m_xBase->x;
  auto& yv = m_yBase->x;
  std::vector<double> px(nx() + 1);
  std::vector<double> py(ny() + 1);
  for(size_t ix = 0; ix < m_xFuns.size(); ++ix)
  {
    const double y = m_yLines[ix];
    for( size_t i = 0 ; i < px.size(); ++i)
    {
      px[i] = fun(xv[i], y);
    }
    m_xFuns[ix].setP( px );
  }
  for(size_t iy = 0; iy < m_yFuns.size(); ++iy)
  {
    const double x = m_xLines[iy];
    for( size_t i = 0 ; i < py.size(); ++i)
    {
      py[i] = fun(x, yv[i]);
    }
    m_yFuns[iy].setP( py );
  }
}

/**
 * Evaluate the function at a point in a 2D space.
 * @param x :: The x co-ordinate
 * @param y :: The y co-ordinate
 */
double Chebfun2DSpline::operator()(const double& x, const double& y)const
{
  auto ix = std::find_if(m_xLines.begin(), m_xLines.end(), [&x](double d)->bool{return d >= x;});
  if ( ix == m_xLines.end() ) return 0.0;
  if ( ix == m_xLines.begin() && x < m_xBase->startX ) return 0.0;

  size_t i = ix - m_xLines.begin();
  if ( m_xLines[i] == x )
  {
    return m_yFuns[i]( y );
  }
  --i;

  auto iy = std::find_if(m_yLines.begin(), m_yLines.end(), [&y](double d)->bool{return d >= y;});
  if ( iy == m_yLines.end() ) return 0.0;
  if ( iy == m_yLines.begin() && y < m_yBase->startX ) return 0.0;

  size_t j = iy - m_yLines.begin();
  if ( m_yLines[j] == y )
  {
    return m_xFuns[j]( x );
  }
  --j;

  std::vector<double> fx(m_xLines.size());
  for(size_t k = 0; k < fx.size(); ++k)
  {
    fx[k] = m_yFuns[k]( y );
  }
  m_xLineFun.setP( fx );

  std::vector<double> fy(m_yLines.size());
  for(size_t k = 0; k < fy.size(); ++k)
  {
    fy[k] = m_xFuns[k]( x );
  }
  m_yLineFun.setP( fy );

  return (m_xLineFun(x) + m_yLineFun(y)) / 2;

}


} // Numeric
