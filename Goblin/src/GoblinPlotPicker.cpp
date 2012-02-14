#include "Goblin/GoblinPlotPicker.h"

namespace Goblin
{

GoblinPlotPicker::GoblinPlotPicker(QtAPI::Plot* plot):
QtAPI::PlotPicker(QtAPI::Plot::xBottom,QtAPI::Plot::yLeft,plot)
{
   setSelectionFlags(QwtPicker::RectSelection);// | QwtPicker::DragSelection);
   setTrackerMode(QwtPicker::AlwaysOn);
}

bool GoblinPlotPicker::accept(QwtPolygon &selection) const
{
  return QtAPI::PlotPicker::accept(selection);
}

void GoblinPlotPicker::begin()
{
  std::cerr << "begin\n";
  QtAPI::PlotPicker::begin();
}

void GoblinPlotPicker::append(const QPoint &p)
{
  std::cerr << "append\n";
  QtAPI::PlotPicker::append(p);
}

void GoblinPlotPicker::move(const QPoint &p)
{
  std::cerr << "move\n";
  QtAPI::PlotPicker::move(p);
}

bool GoblinPlotPicker::end(bool ok)
{
  const QwtPolygon& p = selection();
  QRect r = pickRect();
  std::cerr << "end " << r.left() << ',' << r.top() << ' ' << r.right() << ',' << r.bottom() << std::endl;
  for(int i = 0; i < p.size(); ++i)
  {
    std::cerr << "     " << p[i].x() << ',' << p[i].y() << std::endl;
  }
  return QtAPI::PlotPicker::end(ok);
}


} // namespace Goblin
