#include "Goblin/GoblinPlotPicker.h"

#include <QtGui/QPainter>

namespace Goblin
{

GoblinPlotPicker::GoblinPlotPicker(QtAPI::Plot* plot):
QtAPI::PlotPicker(QtAPI::Plot::xBottom,QtAPI::Plot::yLeft,plot),
m_drawSelf(false)
{
   setSelectionFlags(QwtPicker::RectSelection);// | QwtPicker::DragSelection);
   setTrackerMode(QwtPicker::AlwaysOn);
}

bool GoblinPlotPicker::accept(QwtPolygon &selection) const
{
  return QtAPI::PlotPicker::accept(selection);
}

void GoblinPlotPicker::begin()
{
  m_drawSelf = true;
  QtAPI::PlotPicker::begin();
}

void GoblinPlotPicker::append(const QPoint &p)
{
  QtAPI::PlotPicker::append(p);
}

void GoblinPlotPicker::move(const QPoint &p)
{
  QtAPI::PlotPicker::move(p);
  canvas()->plot()->replot();
}

bool GoblinPlotPicker::end(bool ok)
{
  const QwtPolygon& p = selection();
  QRect r = pickRect();
  std::cerr << "end " << r.left() << ',' << r.top() << ' ' << r.right() << ',' << r.bottom() << std::endl;
  for(int i = 0; i < p.size(); ++i)
  {
    std::cerr << "     " << p[i].x() << ',' << p[i].y() << std::endl;
  }
  m_drawSelf = false;
  canvas()->plot()->replot();
  return QtAPI::PlotPicker::end(ok);
}

void GoblinPlotPicker::draw(QPainter *painter, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRect &canvasRect) const
{
  if (m_drawSelf)
  {
    const QwtPolygon& p = selection();
    if (p.size() != 2) return;
    painter->drawLine(p[0],p[1]);
  }
}

} // namespace Goblin
