#include "Goblin/GoblinPlotPicker.h"
#include "QtAPI/PlotCurve.h"

#include "qwt_scale_map.h"
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>

namespace Goblin
{

GoblinPlotPicker::GoblinPlotPicker(QtAPI::Plot* plot):
QtAPI::PlotPicker(QtAPI::Plot::xBottom,QtAPI::Plot::yLeft,plot),
m_drawSelf(false),
m_currentLine(0)
{
  auto curveList = plot->getCurves();
  if (!curveList.isEmpty())
  {
    m_curve = curveList[0];
    m_linelist = boost::dynamic_pointer_cast<LineList>(m_curve->getWorkspace());
  }
}

void GoblinPlotPicker::draw(QPainter *painter, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRect &canvasRect) const
{
  if (m_drawSelf)
  {
    painter->fillRect(0,0,100,100,Qt::red);
  }
  if (m_linelist)
  {
    double startX = xMap.invTransform(canvasRect.left());
    double endX = xMap.invTransform(canvasRect.right());
    double x = m_linelist->line(m_currentLine);
    double y = m_linelist->height(m_currentLine);

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

void GoblinPlotPicker::widgetWheelEvent(QWheelEvent *we)
{
  if (!m_plot) return;
  //int numDegrees = we->delta() / 8;
  double startX = m_plot->canvasMap(QtAPI::Plot::xBottom).s1();
  double endX = m_plot->canvasMap(QtAPI::Plot::xBottom).s2();
  double dX = (endX - startX)/10;
  if (we->delta() > 0)
  {
    startX += dX;
    endX -= dX;
  }
  else if (we->delta() < 0)
  {
    startX -= dX;
    endX += dX;
  }
  m_plot->setAxisScale(QtAPI::Plot::xBottom,startX,endX);
  m_plot->replot();
}

} // namespace Goblin
