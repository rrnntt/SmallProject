#ifndef TEACHQT_GRAPH_H
#define TEACHQT_GRAPH_H

#include "TeachQt/DllExport.h"
#include "QtAPI/Plot.h"

namespace TeachQt
{

class TEACHQT_EXPORT Graph: public QtAPI::Plot
{
  Q_OBJECT
public:
  Graph();
  ~Graph();
  void setXAxisScale(double min, double max, bool keepRatio = true);
  void setYAxisScale(double min, double max, bool keepRatio = true);
  void correctToSquare();
protected:
  void paintEvent(QPaintEvent*);
  bool m_needSquareCorrection;
};

} // namespace Teach
#endif // TEACHQT_GRAPH_H
