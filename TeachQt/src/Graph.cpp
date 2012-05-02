#include "TeachQt/Graph.h"
#include "QtAPI/PlotRescaler.h"

#include "TeachQt/GraphShape.h"

namespace TeachQt
{

Graph::Graph():
m_needSquareCorrection(false)
{
  setTitle("");
  setAxisTitle(QtAPI::Plot::xBottom,"");
  setAxisTitle(QtAPI::Plot::yLeft,"");
  enableAxis(QtAPI::Plot::xTop);
  enableAxis(QtAPI::Plot::yRight);
  setZoomBase();

  setRescaler(new QtAPI::ConstSizeRescaler(this));
  setXAxisScale(-10,10,false);
  setYAxisScale(-10,10,false);

  correctToSquare();

  auto circle = new GraphCircle;
  circle->attach(this);

}

Graph::~Graph()
{
}

void Graph::setXAxisScale(double min, double max, bool keepRatio)
{
  setAxisScale(xBottom,min,max);
  setAxisScale(xTop,min,max);
  if (keepRatio)
  {
    double dx = getXEnd() - getXStart();
    double dy = getYEnd() - getYStart();
    double ratio = dy / dx;
    dx = max - min;
    dy = dx * ratio / 2;
    double y = (getYEnd() + getYStart()) / 2;
    setXAxisScale(y - dy, y + dy, false);
  }
} 

void Graph::setYAxisScale(double min, double max, bool keepRatio)
{
  setAxisScale(yLeft,min,max);
  setAxisScale(yRight,min,max);
  if (keepRatio)
  {
    double dx = getXEnd() - getXStart();
    double dy = getYEnd() - getYStart();
    double ratio = dy / dx;
    dy = max - min;
    dx = dy / ratio / 2;
    double x = (getXEnd() + getXStart()) / 2;
    setXAxisScale(x - dx, x + dx, false);
  }
}

void Graph::correctToSquare()
{
  auto xMap = canvasMap(QtAPI::Plot::xBottom);
  auto yMap = canvasMap(QtAPI::Plot::yLeft);

  double xRatio = xMap.sDist() / xMap.pDist();

  if (xRatio == 0)
  {
    m_needSquareCorrection = true;
    return;
  }

  double ySDist = xRatio * yMap.pDist();

  double y0 = (yMap.s1() + yMap.s2()) / 2;
  setYAxisScale(y0 - ySDist/2, y0 + ySDist/2,false);

  std::cerr << y0 - ySDist/2 << ' ' << y0 + ySDist/2 << std::endl;
}

void Graph::paintEvent(QPaintEvent* e)
{
  Plot::paintEvent(e);
  if (m_needSquareCorrection)
  {
    correctToSquare();
    m_needSquareCorrection = false;
    delayedReplot();
  }
}

} // namespace Teach
