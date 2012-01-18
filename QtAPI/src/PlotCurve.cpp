#include "QtAPI/PlotCurve.h"

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

boost::shared_ptr<PlotCurve> PlotCurve::create()
{
  return boost::shared_ptr<PlotCurve>(new PlotCurve());
}

boost::shared_ptr<PlotCurve> PlotCurve::create(const QString &title)
{
  return boost::shared_ptr<PlotCurve>(new PlotCurve(title));
}

} // QtAPI
