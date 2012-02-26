#ifndef TEACH_GRAPH_H
#define TEACH_GRAPH_H

#include "Teach/DllExport.h"
#include "QtAPI/Plot.h"

#include "qwt_plot_rescaler.h"

namespace Teach
{

class TEACH_EXPORT Graph: public QtAPI::Plot
{
  Q_OBJECT
public:
  Graph();
  ~Graph();
  void setXAxisScale(double min, double max, bool keepRatio = true);
  void setYAxisScale(double min, double max, bool keepRatio = true);
  void correctAxisToAspectRatio();
protected:
  QwtPlotRescaler *m_rescaler;
};

} // namespace Teach
#endif // TEACH_GRAPH_H
