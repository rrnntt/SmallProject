#ifndef QTAPI_PLOTRESCALER_H
#define QTAPI_PLOTRESCALER_H

#include "QtAPI/DllExport.h"
#include "QtAPI/Plot.h"

#include <QtCore/QSize>

namespace QtAPI
{

class QTAPI_EXPORT PlotRescaler
{
public:
  PlotRescaler(Plot* plot):m_plot(plot){}
  ~PlotRescaler(){}
  virtual void rescale(const QSize& oldSize, const QSize& newSize) const = 0;
protected:
  Plot* m_plot;
};

class QTAPI_EXPORT ConstSizeRescaler: public PlotRescaler
{
public:
  ConstSizeRescaler(Plot* plot):PlotRescaler(plot){}
  virtual void rescale(const QSize& oldSize, const QSize& newSize) const;
};

} // namespace QtAPI
#endif // QTAPI_PLOTRESCALER_H
