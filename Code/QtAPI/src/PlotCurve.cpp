#include "QtAPI/PlotCurve.h"

#include <iostream>

namespace QtAPI
{

PlotCurve::PlotCurve():
QwtPlotCurve()
{
}

PlotCurve::PlotCurve(const QString &title):
QwtPlotCurve(title)
{
}

PlotCurve::~PlotCurve()
{
  //std::cerr << "Curve deleted\n";
}

QPointer<PlotCurve> PlotCurve::create()
{
  return QPointer<PlotCurve>(new PlotCurve());
}

QPointer<PlotCurve> PlotCurve::create(const QString &title)
{
  return QPointer<PlotCurve>(new PlotCurve(title));
}

} // QtAPI
