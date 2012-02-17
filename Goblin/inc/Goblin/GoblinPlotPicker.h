#ifndef GOBLIN_GOBLINPLOTPICKER_H
#define GOBLIN_GOBLINPLOTPICKER_H

#include "Goblin/DllExport.h"
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

  bool m_drawSelf;
};

} // namespace Goblin
#endif // GOBLIN_GOBLINPLOTPICKER_H
