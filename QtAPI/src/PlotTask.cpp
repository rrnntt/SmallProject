#include "QtAPI/PlotTask.h"
#include "QtAPI/Table.h"
#include "QtAPI/Plot.h"
#include "QtAPI/CurveManager.h"
#include "QtAPI/PlotCurve.h"
#include "QtAPI/WindowManager.h"

#include "DataObjects/TableWorkspace.h"
#include "DataObjects/NumericColumn.h"

namespace QtAPI
{

PlotTask::PlotTask()
{
}

QMenu* PlotTask::menu(SubWindow* w) const
{
  return nullptr;
}

QPointer<Plot> PlotTask::showPlot(const Table* table, 
  const std::string& columnX, 
  const std::string& columnY, 
  const std::string& columnE) const
{
  return showPlot(table->getWorkspace(),columnX,columnY,columnE);
}

QPointer<Plot> PlotTask::showPlot(boost::shared_ptr<DataObjects::TableWorkspace> tws,
  const std::string& columnX,
  const std::string& columnY,
  const std::string& columnE) const
{
  auto colX = tws->getColumn(columnX)->asNumeric();
  auto colY = tws->getColumn(columnY)->asNumeric();

  PlotCurve_ptr curve = PlotCurve::create();
  std::vector<double> x,y;
  colX->fill(x);
  colY->fill(y);
  curve->setData(&x[0],&y[0],x.size());
  CurveManager::instance().addCurve(curve);
  auto plot = new Plot();
  curve->attach(plot);

  WindowManager::instance().newSubWindow(plot);
  return plot;
}

} // namespace QtAPI
