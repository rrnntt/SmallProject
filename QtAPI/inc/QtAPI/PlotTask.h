#ifndef QTAPI_PLOTTASK_H
#define QTAPI_PLOTTASK_H

#include "QtAPI/DllExport.h"
#include "QtAPI/Task.h"

#include <QtCore/QPointer>

namespace QtAPI
{

class Table;
class Plot;

class QTAPI_EXPORT PlotTask: public Task
{
public:
  PlotTask();
  ~PlotTask(){}
  virtual Type type() const {return Background;}
  virtual QMenu* menu(SubWindow* w = nullptr) const;
  virtual int menuOrder() const {return customMenuOder;}

  QPointer<Plot> showPlot(const Table* table, 
    const std::string& columnX, 
    const std::string& columnY, 
    const std::string& columnE) const;
protected:
  QMenu* tableMenu(const Table* table) const;
};

} // namespace QtAPI
#endif // QTAPI_PLOTTASK_H
