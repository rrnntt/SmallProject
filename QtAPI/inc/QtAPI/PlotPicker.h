#ifndef QTAPI_PLOTPICKER_H
#define QTAPI_PLOTPICKER_H

#include "QtAPI/DllExport.h"
#include "qwt_plot_picker.h"

namespace QtAPI
{

  class QTAPI_EXPORT PlotPicker: public QwtPlotPicker
{
public:
  PlotPicker(int xAxis, int yAxis,QwtPlotCanvas *canvas);
  ~PlotPicker(){}
protected:
  // Overriden from QwtPlotPicker
  virtual bool accept(QwtPolygon &selection) const;
  virtual void begin();
  virtual void append(const QPoint &);
  virtual void move(const QPoint &);
  virtual bool end(bool ok = true);
};

} // namespace QtAPI
#endif // QTAPI_PLOTPICKER_H
