#include "Teach/GraphShape.h"

#include "qwt_scale_map.h"

#include <QtGui/QPainter>

#include <iostream>

namespace Teach
{

GraphShape::GraphShape()
{
}

void GraphShape::beginPainting(QPainter *painter)const
{
  painter->save();
  painter->setPen(m_pen);
}

void GraphShape::endPainting(QPainter *painter)const
{
  painter->restore();
}

//--------------------------------------

void GraphPoint::draw(QPainter *painter, 
    const QwtScaleMap &xMap, const QwtScaleMap &yMap,
    const QRect &canvasRect) const
{
  int x = xMap.transform(m_p.x());
  int y = yMap.transform(m_p.y());

  beginPainting(painter);
  painter->fillRect(x-m_size,y-m_size,m_size,m_size,m_pen.color());
  endPainting(painter);
}

//--------------------------------------

GraphCircle::GraphCircle():
m_radius(1.0),
m_centre(0,0)
{
}

void GraphCircle::draw(QPainter *painter, 
    const QwtScaleMap &xMap, const QwtScaleMap &yMap,
    const QRect &canvasRect) const
{
  int x = xMap.transform(m_centre.x());
  int y = yMap.transform(m_centre.y());

  int r = xMap.transform(m_centre.x() + m_radius);
  r -= x;
  int ry = yMap.transform(m_centre.y() + m_radius);
  ry = y - ry;

  if (abs(r - ry) > 1)
  {
    std::cerr << "circle stretch " << r - ry << std::endl;
  }

  beginPainting(painter);
  painter->drawEllipse(QPoint(x,y),r,r);
  endPainting(painter);
}

//------------------------------------------------------------------

void GraphLineSegment::draw(QPainter *painter, 
    const QwtScaleMap &xMap, const QwtScaleMap &yMap,
    const QRect &canvasRect) const
{
  int x1 = xMap.transform(m_p1.x());
  int y1 = yMap.transform(m_p1.y());
  int x2 = xMap.transform(m_p2.x());
  int y2 = yMap.transform(m_p2.y());

  beginPainting(painter);
  painter->drawLine(x1,y1,x2,y2);
  endPainting(painter);
}

//------------------------------------------------------------------

} // namespace Teach
