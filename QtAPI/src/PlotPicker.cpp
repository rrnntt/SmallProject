#include "QtAPI/PlotPicker.h"

#include <iostream>

namespace QtAPI
{

PlotPicker::PlotPicker(int xAxis, int yAxis,QwtPlotCanvas *canvas):
QwtPlotPicker(xAxis,yAxis,canvas)
{
}

bool PlotPicker::accept(QwtPolygon &selection) const
{
  return QwtPlotPicker::accept(selection);
}

void PlotPicker::begin()
{
  std::cerr << "begin\n";
  QwtPlotPicker::begin();
}

void PlotPicker::append(const QPoint &p)
{
  std::cerr << "append\n";
  QwtPlotPicker::append(p);
}

void PlotPicker::move(const QPoint &p)
{
  std::cerr << "move\n";
  QwtPlotPicker::move(p);
}

bool PlotPicker::end(bool ok)
{
  std::cerr << "end\n";
  return QwtPlotPicker::end(ok);
}


} // namespace QtAPI
