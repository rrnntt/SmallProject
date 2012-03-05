#ifndef TEACH_GRAPH_H
#define TEACH_GRAPH_H

#include "Teach/DllExport.h"
#include "QtAPI/Plot.h"

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
  void correctToSquare();
protected:
  void paintEvent(QPaintEvent*);
  bool m_needSquareCorrection;
};

} // namespace Teach
#endif // TEACH_GRAPH_H
