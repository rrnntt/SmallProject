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
  virtual bool accept(QwtPolygon &selection) const;
  virtual void begin();
  virtual void append(const QPoint &);
  virtual void move(const QPoint &);
  virtual bool end(bool ok = true);
  virtual void draw(QPainter *painter, 
      const QwtScaleMap &xMap, const QwtScaleMap &yMap,
      const QRect &canvasRect) const;

  bool m_drawSelf;
};

} // namespace Goblin
#endif // GOBLIN_GOBLINPLOTPICKER_H
