#include "QtAPI/PlotPicker.h"
#include "QtAPI/Plot.h"
#include "qwt_plot_canvas.h"

#include <QEvent>
#include <iostream>

namespace QtAPI
{

PlotPicker::PlotPicker(Plot::Axis xAxis, Plot::Axis yAxis,Plot* plot):
QObject(plot),
m_plot(plot),
m_xAxis(xAxis),
m_yAxis(yAxis),
m_isEnabled(false)
{
}

void PlotPicker::setEnabled(bool on)
{
  if (!m_plot) return;
  if ( m_isEnabled != on )
  {
    m_isEnabled = on;

    if ( m_isEnabled )
    {
      m_plot->canvas()->installEventFilter(this);
      this->attach(m_plot);
    }
    else
    {
      m_plot->canvas()->removeEventFilter(this);
      this->detach();
    }
  }
}

/**
 * Default draw: do nothing.
 */
void PlotPicker::draw(QPainter *painter, 
    const QwtScaleMap &xMap, const QwtScaleMap &yMap,
    const QRect &canvasRect) const
{
}

/*!
  \brief Event filter

  When isEnabled() the mouse events of the observed widget are filtered.

  \sa widgetMousePressEvent(), widgetMouseReleaseEvent(),
      widgetMouseMoveEvent(), widgetWheelEvent(), widgetKeyPressEvent()
      widgetKeyReleaseEvent()
*/
bool PlotPicker::eventFilter(QObject *o, QEvent *e)
{
  if (!m_plot)
  {
    return QObject::eventFilter(o, e);
  }
  auto canvas = m_plot->canvas();
  if ( o && o == qobject_cast<QObject*>(canvas) )
  {
    switch(e->type() )
    {
    case QEvent::MouseButtonDblClick:
      {
        widgetMouseDoubleClickEvent((QMouseEvent *)e);
        break;
      }
    case QEvent::MouseButtonPress:
      {
        widgetMousePressEvent((QMouseEvent *)e);
        break;
      }
    case QEvent::MouseMove:
      {
        widgetMouseMoveEvent((QMouseEvent *)e);
        break;
      }
    case QEvent::MouseButtonRelease:
      {
        widgetMouseReleaseEvent((QMouseEvent *)e);
        break;
      }
    case QEvent::Wheel:
      {
        widgetWheelEvent((QWheelEvent *)e);
        break;
      }
    case QEvent::KeyPress:
      {
        widgetKeyPressEvent((QKeyEvent *)e);
        break;
      }
    case QEvent::KeyRelease:
      {
        widgetKeyReleaseEvent((QKeyEvent *)e);
        break;
      }
    default:;
    }
  }
  return QObject::eventFilter(o, e);
}

void PlotPicker::widgetMousePressEvent(QMouseEvent *)
{
}

void PlotPicker::widgetMouseReleaseEvent(QMouseEvent *)
{
}

void PlotPicker::widgetMouseMoveEvent(QMouseEvent *)
{
}

void PlotPicker::widgetWheelEvent(QWheelEvent *)
{
}

void PlotPicker::widgetKeyPressEvent(QKeyEvent *)
{
}

void PlotPicker::widgetKeyReleaseEvent(QKeyEvent *)
{
}


} // namespace QtAPI
