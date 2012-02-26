#include "Teach/Graph.h"

namespace Teach
{

Graph::Graph()
{
  setTitle("");
  setAxisTitle(QtAPI::Plot::xBottom,"");
  setAxisTitle(QtAPI::Plot::yLeft,"");
  enableAxis(QtAPI::Plot::xTop);
  enableAxis(QtAPI::Plot::yRight);
  setXAxisScale(-10,10,false);
  setYAxisScale(-10,10,false);
  setZoomBase();

  m_rescaler = new QwtPlotRescaler(canvas());
  //m_rescaler->setEnabled(true);

}

Graph::~Graph()
{
  //delete m_rescaler;
}

void Graph::setXAxisScale(double min, double max, bool keepRatio)
{
  double dx = getXEnd() - getXStart();
  double dy = getYEnd() - getYStart();
  double ratio = dy / dx;
  setAxisScale(xBottom,min,max);
  setAxisScale(xTop,min,max);
  if (keepRatio)
  {
    dx = max - min;
    dy = dx * ratio / 2;
    double y = (getYEnd() + getYStart()) / 2;
    setXAxisScale(y - dy, y + dy, false);
  }
}

void Graph::setYAxisScale(double min, double max, bool keepRatio)
{
  double dx = getXEnd() - getXStart();
  double dy = getYEnd() - getYStart();
  double ratio = dy / dx;
  setAxisScale(yLeft,min,max);
  setAxisScale(yRight,min,max);
  if (keepRatio)
  {
    dy = max - min;
    dx = dy / ratio / 2;
    double x = (getXEnd() + getXStart()) / 2;
    setXAxisScale(x - dx, x + dx, false);
  }
}

void Graph::correctAxisToAspectRatio()
{
}

} // namespace Teach
