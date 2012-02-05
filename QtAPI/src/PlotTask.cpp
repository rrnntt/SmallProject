#include "QtAPI/PlotTask.h"
#include "QtAPI/Table.h"
#include "QtAPI/Plot.h"
#include "QtAPI/CurveManager.h"
#include "QtAPI/PlotCurve.h"

#include "DataObjects/TableWorkspace.h"

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

QPointer<Plot> PlotTask::showPlot(boost::shared_ptr<TableWorkspace> tws,
  const std::string& columnX,
  const std::string& columnY,
  const std::string& columnE) const
{
}

} // namespace QtAPI
