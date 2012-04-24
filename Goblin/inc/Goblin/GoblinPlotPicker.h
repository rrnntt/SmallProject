#ifndef GOBLIN_GOBLINPLOTPICKER_H
#define GOBLIN_GOBLINPLOTPICKER_H

#include "Goblin/DllExport.h"
#include "Goblin/LineList.h"
#include "QtAPI/PlotPicker.h"
#include "QtAPI/PlotObject.h"

namespace Goblin
{

class GOBLIN_EXPORT GoblinPlotPicker: public QtAPI::PlotPicker
{
public:
  GoblinPlotPicker(QtAPI::Plot* plot);
  ~GoblinPlotPicker(){}
protected:
  virtual void draw(QPainter *painter, 
      const QwtScaleMap &xMap, const QwtScaleMap &yMap,
      const QRect &canvasRect) const;

  virtual void widgetMousePressEvent(QMouseEvent *);
  virtual void widgetMouseReleaseEvent(QMouseEvent *);
  virtual void widgetMouseMoveEvent(QMouseEvent *);
  virtual void widgetMouseDoubleClickEvent(QMouseEvent *);
  virtual void widgetWheelEvent(QWheelEvent *);
  virtual void widgetKeyPressEvent(QKeyEvent *);

  bool m_drawSelf;
  //QPointer<QtAPI::FunctionCurve> m_curve;
  QtAPI::PlotObject::id_t m_curve_id;
  LineList_ptr m_linelist;
  mutable size_t m_currentLine;
  bool m_shifting;
  QPoint m_p0;
};

} // namespace Goblin
#endif // GOBLIN_GOBLINPLOTPICKER_H
