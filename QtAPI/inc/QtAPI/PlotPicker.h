#ifndef QTAPI_PLOTPICKER_H
#define QTAPI_PLOTPICKER_H

#include "QtAPI/DllExport.h"
#include "QtAPI/Plot.h"
#include "qwt_plot_picker.h"
#include "qwt_plot_item.h"

namespace QtAPI
{

  class QTAPI_EXPORT PlotPicker: public QwtPlotPicker, public QwtPlotItem
{
public:
  PlotPicker(int xAxis, int yAxis,Plot* plot);
  ~PlotPicker(){}
protected:
  // Overriden from QwtPlotPicker
  //virtual bool accept(QwtPolygon &selection) const;
  //virtual void begin();
  //virtual void append(const QPoint &);
  //virtual void move(const QPoint &);
  //virtual bool end(bool ok = true);
};

} // namespace QtAPI
#endif // QTAPI_PLOTPICKER_H
