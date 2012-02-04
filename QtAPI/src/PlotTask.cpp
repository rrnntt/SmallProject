#include "QtAPI/PlotTask.h"
#include "QtAPI/Table.h"

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
}

} // namespace QtAPI
