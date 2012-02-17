#include "Goblin/GoblinPlotPicker.h"

#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>

namespace Goblin
{

GoblinPlotPicker::GoblinPlotPicker(QtAPI::Plot* plot):
QtAPI::PlotPicker(QtAPI::Plot::xBottom,QtAPI::Plot::yLeft,plot),
m_drawSelf(false)
{
}

void GoblinPlotPicker::draw(QPainter *painter, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRect &canvasRect) const
{
  if (m_drawSelf)
  {
    painter->fillRect(0,0,100,100,Qt::red);
  }
}

void GoblinPlotPicker::widgetMousePressEvent(QMouseEvent *me)
{
  if (me->button() == Qt::LeftButton)
  {
    m_drawSelf = true;
    m_plot->replot();
  }
  if (me->button() == Qt::RightButton)
  {
    m_drawSelf = false;
    m_plot->replot();
  }
}


} // namespace Goblin
