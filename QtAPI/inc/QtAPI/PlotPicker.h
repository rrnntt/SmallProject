#ifndef QTAPI_PLOTPICKER_H
#define QTAPI_PLOTPICKER_H

#include "QtAPI/DllExport.h"
#include "QtAPI/Plot.h"
#include "qwt_plot_item.h"

#include <QtCore/QPointer>

namespace QtAPI
{

class QTAPI_EXPORT PlotPicker:public QObject, public QwtPlotItem
{
  Q_OBJECT
public:
  PlotPicker(Plot::Axis xAxis, Plot::Axis yAxis,Plot* plot);
  ~PlotPicker(){}
  void setEnabled(bool on);
  bool isEnabled() const {return m_isEnabled;}
protected:
  virtual void draw(QPainter *painter, 
      const QwtScaleMap &xMap, const QwtScaleMap &yMap,
      const QRect &canvasRect) const;
  
  virtual bool eventFilter(QObject *, QEvent *);

  virtual void widgetMousePressEvent(QMouseEvent *);
  virtual void widgetMouseReleaseEvent(QMouseEvent *);
  virtual void widgetMouseMoveEvent(QMouseEvent *);
  virtual void widgetMouseDoubleClickEvent(QMouseEvent *){}

  virtual void widgetWheelEvent(QWheelEvent *);
  virtual void widgetKeyPressEvent(QKeyEvent *);
  virtual void widgetKeyReleaseEvent(QKeyEvent *);

  QPointer<Plot> m_plot;
  Plot::Axis m_xAxis;
  Plot::Axis m_yAxis;
  bool m_isEnabled;
};

} // namespace QtAPI
#endif // QTAPI_PLOTPICKER_H
