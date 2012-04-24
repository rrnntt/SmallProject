#include "Goblin/GoblinPlotPicker.h"
#include "QtAPI/FunctionCurve.h"

#include "qwt_scale_map.h"
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtGui/QFontMetrics>

namespace Goblin
{

GoblinPlotPicker::GoblinPlotPicker(QtAPI::Plot* plot):
QtAPI::PlotPicker(QtAPI::Plot::xBottom,QtAPI::Plot::yLeft,plot),
m_drawSelf(false),
m_currentLine(0),
m_shifting(false)
{
  auto curveIdList = plot->getCurveIDs();
  if (!curveIdList.isEmpty())
  {
    m_curve_id = curveIdList[0];
    auto curve = plot->getCurve(m_curve_id);
    if ( curve )
    {
      m_linelist = boost::dynamic_pointer_cast<LineList>(curve->getWorkspace());
    }
  }
}

void GoblinPlotPicker::draw(QPainter *painter, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRect &canvasRect) const
{
  if (m_linelist)
  {
    double startX = xMap.invTransform(canvasRect.left());
    double endX = xMap.invTransform(canvasRect.right());
    double x = m_linelist->line(m_currentLine);
    if (x < startX)
    {
      m_currentLine = m_linelist->findNearest(startX);
      if (m_linelist->line(m_currentLine) < startX && m_currentLine < m_linelist->size() - 1)
      {
        ++m_currentLine;
      }
    }
    if (x > endX)
    {
      m_currentLine = m_linelist->findNearest(startX);
      if (m_linelist->line(m_currentLine) > endX && m_currentLine > 0)
      {
        --m_currentLine;
      }
    }
    x = m_linelist->line(m_currentLine);
    double y = m_linelist->height(m_currentLine);
    int x1 = xMap.transform(x);
    int y1 = yMap.transform(y) - 1;
    int y2 = yMap.p2();
    painter->setPen(Qt::red); // <-----------------------------!
    painter->drawLine(x1,y1,x1,y2);
    auto curve = m_plot->getCurve(m_curve_id);
    if ( curve )
    {
      painter->setPen(curve->pen()); // <-----------------------------!
    }
    QFontMetrics fm(m_plot->font());
    
    painter->drawText(0,fm.height(),QString::fromStdString(m_linelist->lineString(m_currentLine)));
  }
}

void GoblinPlotPicker::widgetMousePressEvent(QMouseEvent *me)
{
  if (me->button() == Qt::LeftButton)
  {
    m_drawSelf = true;
    m_shifting = true;
    m_p0 = me->pos();
    m_plot->replot();
  }
  if (me->button() == Qt::RightButton)
  {
    m_drawSelf = false;
    m_plot->replot();
  }
}

void GoblinPlotPicker::widgetMouseReleaseEvent(QMouseEvent *me)
{
  m_shifting = false;
}

void GoblinPlotPicker::widgetMouseMoveEvent(QMouseEvent *me)
{
  auto p = me->pos();
  double x0 = m_plot->canvasMap(QtAPI::Plot::xBottom).invTransform(m_p0.x());
  double x = m_plot->canvasMap(QtAPI::Plot::xBottom).invTransform(p.x());
  double y0 = m_plot->canvasMap(QtAPI::Plot::xBottom).invTransform(m_p0.y());
  double y = m_plot->canvasMap(QtAPI::Plot::xBottom).invTransform(p.y());

  double startX = m_plot->getXStart();
  double endX = m_plot->getXEnd();
  m_plot->setAxisScale(QtAPI::Plot::xBottom,startX - x + x0,endX - x + x0);
  m_plot->replot();
  m_p0 = p;
}

void GoblinPlotPicker::widgetMouseDoubleClickEvent(QMouseEvent *me)
{
  if (!m_plot || !m_linelist) return;
  auto p = me->pos();
  double x = m_plot->canvasMap(QtAPI::Plot::xBottom).invTransform(p.x());
  m_currentLine = m_linelist->findNearest(x);
  m_plot->replot();
}

void GoblinPlotPicker::widgetWheelEvent(QWheelEvent *we)
{
  if (!m_plot) return;
  //int numDegrees = we->delta() / 8;
  if (we->modifiers() & Qt::CTRL)
  {
    double startY = m_plot->getYStart();
    double endY = m_plot->getYEnd();
    double dY = (endY - startY)/10;
    if (we->delta() > 0)
    {
      //startY += dY;
      endY -= dY;
    }
    else if (we->delta() < 0)
    {
      //startY -= dY;
      endY += dY;
    }
    m_plot->setAxisScale(QtAPI::Plot::yLeft,startY,endY);
  }
  else
  {
    double startX = m_plot->getXStart();
    double endX = m_plot->getXEnd();
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
  }
  m_plot->replot();
}

void GoblinPlotPicker::widgetKeyPressEvent(QKeyEvent *ke)
{
  if (ke->key() == Qt::Key_Tab && m_plot)
  {
    auto new_id = m_plot->bringForwardNextCurve(m_curve_id);
    auto curve = m_plot->getCurve(new_id);
    if (curve)
    {
      m_curve_id = new_id;
      m_linelist = boost::dynamic_pointer_cast<LineList>(curve->getWorkspace());
      std::cerr << m_linelist->name() << std::endl;
      m_plot->replot();
    }
  }
}


} // namespace Goblin
