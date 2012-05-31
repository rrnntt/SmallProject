#include "Numeric/Chebfun2DSpline.h"

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
m_yBase(new ChebfunBase(ny, startY, endY))
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

} // Numeric
