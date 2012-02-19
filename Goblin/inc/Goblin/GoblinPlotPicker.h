#ifndef GOBLIN_GOBLINPLOTPICKER_H
#define GOBLIN_GOBLINPLOTPICKER_H

#include "Goblin/DllExport.h"
#include "Goblin/LineList.h"
#include "QtAPI/PlotPicker.h"

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
  virtual void widgetWheelEvent(QWheelEvent *);

  bool m_drawSelf;
  QPointer<QtAPI::PlotCurve> m_curve;
  LineList_ptr m_linelist;
  size_t m_currentLine;
};

} // namespace Goblin
#endif // GOBLIN_GOBLINPLOTPICKER_H
