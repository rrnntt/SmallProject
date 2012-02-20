#ifndef QTAPI_PLOTTASK_H
#define QTAPI_PLOTTASK_H

#include "QtAPI/DllExport.h"
#include "QtAPI/Task.h"

#include <QtCore/QPointer>

class QActionGroup;

namespace DataObjects
{
  class TableWorkspace;
}

namespace QtAPI
{

class Table;
class Plot;

class QTAPI_EXPORT PlotTask: public Task
{
  Q_OBJECT
public:
  PlotTask();
  ~PlotTask(){}
  virtual Type type() const {return Background;}
  virtual QMenu* menu(SubWindow* w = nullptr) const;
  virtual int menuOrder() const {return customMenuOder;}

  QPointer<Plot> showPlot(const Table* table,
    const std::string& columnX,
    const std::string& columnY,
    const std::string& columnE = "") const;

  QPointer<Plot> showPlot(boost::shared_ptr<DataObjects::TableWorkspace> tws,
    const std::string& columnX,
    const std::string& columnY,
    const std::string& columnE = "") const;
  QPointer<Plot> showPlot(boost::shared_ptr<DataObjects::TableWorkspace> tws,
    std::vector<std::string>& columnNames) const;

  void addTableToPlot(Plot* plot, boost::shared_ptr<DataObjects::TableWorkspace> tws,
    std::vector<std::string>& columnNames) const;

protected slots:
  void showTablePlot();
  void setZoomer();
  void setMagnifier();
  void setCustomPicker();
  void showPlotDialog();
  void menuAboutToShow();
protected:
  QMenu* tableMenu(const Table* table) const;

  mutable QPointer<Table> m_table; ///< table in current sub-window
  mutable QPointer<Plot> m_plot; ///< plot in current sub-window
  QAction *m_showTablePlot;
  QActionGroup* m_pickerGroup; ///< group for selecting a picker
  QAction *m_setZoomer;    ///< select zoomer
  QAction *m_setMagnifier; ///< select magnifier
  QAction *m_setCustomPicker; ///< select ???
  QAction *m_showPlotDialog; ///< show plot settings dialog
};

} // namespace QtAPI
#endif // QTAPI_PLOTTASK_H
