#include "QtAPI/PlotTask.h"
#include "QtAPI/Table.h"
#include "QtAPI/Plot.h"
#include "QtAPI/CurveManager.h"
#include "QtAPI/FunctionCurve.h"
#include "QtAPI/WindowManager.h"
#include "QtAPI/PlotDialog.h"
#include "QtAPI/PlotCurve.h"

#include "DataObjects/TableWorkspace.h"
#include "DataObjects/NumericColumn.h"

#include <QtGui/QMenu>
#include <QtGui/QActionGroup>

namespace QtAPI
{

PlotTask::PlotTask()
{
  m_showTablePlot = new QAction("Plot columns",this);
  connect(m_showTablePlot,SIGNAL(triggered()),this,SLOT(showTablePlot()));
  addAction("PlotColumns",m_showTablePlot);

  m_setZoomer = new QAction("Zoom",this);
  m_setZoomer->setCheckable(true);
  connect(m_setZoomer,SIGNAL(triggered()),this,SLOT(setZoomer()));
  addAction("Zoom",m_setZoomer);

  m_setMagnifier = new QAction("Magnify",this);
  m_setMagnifier->setCheckable(true);
  connect(m_setMagnifier,SIGNAL(triggered()),this,SLOT(setMagnifier()));
  addAction("Magnify",m_setMagnifier);

  m_setCustomPicker = new QAction("Custom",this);
  m_setCustomPicker->setCheckable(true);
  connect(m_setCustomPicker,SIGNAL(triggered()),this,SLOT(setCustomPicker()));
  addAction("Custom",m_setCustomPicker);

  m_pickerGroup = new  QActionGroup(this);
  m_pickerGroup->addAction(m_setMagnifier);
  m_pickerGroup->addAction(m_setZoomer);
  m_pickerGroup->addAction(m_setCustomPicker);

  m_showPlotDialog = new QAction("Plot settings...",this);
  connect(m_showPlotDialog,SIGNAL(triggered()),this,SLOT(showPlotDialog()));
  addAction("PlotSettings",m_showPlotDialog);
}

QMenu* PlotTask::menu(SubWindow* w) const
{
  m_table = nullptr;
  m_plot = nullptr;
  if (!w) return nullptr;
  m_table = qobject_cast<Table*>(w->widget());
  if (m_table) // current sub-window is a table
  {
    QMenu* theMenu = new QMenu("Plot");
    theMenu->addAction(m_showTablePlot);
    return theMenu;
  }
  m_plot = qobject_cast<Plot*>(w->widget());
  if (m_plot) // current sub-window is a plot
  {
    if (m_plot->isZoomerEnabled()) m_setZoomer->setChecked(true);
    if (m_plot->isMagnifierEnabled()) m_setMagnifier->setChecked(true);
    if (!m_plot->hasCustomPicker())
    {
      m_setCustomPicker->setEnabled(false);
    }
    else
    {
      m_setCustomPicker->setEnabled(true);
      if (m_plot->isCustomPickerEnabled()) m_setCustomPicker->setChecked(true);
    }
    QMenu* theMenu = new QMenu("Plot");
    theMenu->addAction(m_showPlotDialog);
    theMenu->addSeparator();
    theMenu->addActions(m_pickerGroup->actions());
    connect(theMenu,SIGNAL(aboutToShow()),this,SLOT(menuAboutToShow()));
    return theMenu;
  }
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

  FunctionCurve* curve = new FunctionCurve;
  std::vector<double> x,y;
  colX->fill(x);
  colY->fill(y);
  curve->setData(x,y);
  curve->setWorkspace(tws);
  auto plot = new Plot();
  plot->addCurve(curve);

  //curve->setTitle(QString::fromStdString(tws->name() + "_" + columnX + "_" + columnY));
  plot->setTitle(QString::fromStdString(tws->name()));
  plot->setAxisTitle(Plot::xBottom,QString::fromStdString(columnX));
  plot->setAxisTitle(Plot::yLeft,QString::fromStdString(columnY));
  plot->updateAxes();
  plot->setZoomBase();

  WindowManager::instance().createSubWindow(plot);
  return plot;
}

/**
 * Show plot and draw curves for columns in columnNames. The columns must have Y roles assigned.
 */
QPointer<Plot> PlotTask::showPlot(boost::shared_ptr<DataObjects::TableWorkspace> tws,
  std::vector<std::string>& columnNames) const
{
  // find first column with th X role
  std::vector<std::string> allNames = m_table->getWorkspace()->getColumnNames();
  DataObjects::NumericColumn* columnX = nullptr;
  std::string columnXName;
  for(auto columnName = allNames.begin(); columnName != allNames.end(); ++columnName)
  {
    auto column = m_table->getWorkspace()->getColumn(*columnName);
    auto colNum = column->asNumeric();
    if (!colNum) continue;
    if (colNum->getPlotRole() == DataObjects::NumericColumn::X)
    {
      columnX = colNum;
      columnXName = *columnName;
      break;
    }
  }
  if (!columnX)
  {
    errorMessage("No X column defined in the table.");
    return nullptr;
  }
  std::string yAxisTitle; 
  auto plot = new Plot();
  plot->setTitle(QString::fromStdString(tws->name()));
  plot->setAxisTitle(Plot::xBottom,QString::fromStdString(columnXName));
  for(auto columnName = columnNames.begin(); columnName != columnNames.end(); ++columnName)
  {
    auto column = m_table->getWorkspace()->getColumn(*columnName);
    auto columnY = column->asNumeric();
    if (!columnY) continue;
    if (columnY->getPlotRole() != DataObjects::NumericColumn::Y)
    {
      errorMessage("Column "+*columnName+" does not have the Y plot role. Skipping.");
      continue;
    }
    FunctionCurve* curve = new FunctionCurve;
    std::vector<double> x,y;
    columnX->fill(x);
    columnY->fill(y);
    curve->setData(x,y);
    curve->setWorkspace(tws);
    plot->addCurve(curve);
    curve->setCurveStyle((FunctionCurve::CurveStyle)m_table->getWorkspace()->getCurveStyle());
    //curve->setTitle(QString::fromStdString(tws->name() + "_" + columnXName + "_" + *columnName));
    if (yAxisTitle.empty())
    {
      yAxisTitle = *columnName;
    }
    else
    {
      yAxisTitle = "Y axis";
    }
    //curve->setWorkspace(tws);
  }
  plot->setAxisTitle(Plot::yLeft,QString::fromStdString(yAxisTitle));
  plot->updateAxes();
  plot->setZoomBase();
  WindowManager::instance().createSubWindow(plot);
  return plot;
}

void PlotTask::addTableToPlot(Plot* plot, boost::shared_ptr<DataObjects::TableWorkspace> tws,
  std::vector<std::string>& columnNames) const
{
  // find first column with th X role
  std::vector<std::string> allNames = tws->getColumnNames();
  DataObjects::NumericColumn* columnX = nullptr;
  std::string columnXName;
  for(auto columnName = allNames.begin(); columnName != allNames.end(); ++columnName)
  {
    auto column = tws->getColumn(*columnName);
    auto colNum = column->asNumeric();
    if (!colNum) continue;
    if (colNum->getPlotRole() == DataObjects::NumericColumn::X)
    {
      columnX = colNum;
      columnXName = *columnName;
      break;
    }
  }
  if (!columnX)
  {
    errorMessage("No X column defined in the table.");
    return;
  }
  std::string yAxisTitle; 
  for(auto columnName = columnNames.begin(); columnName != columnNames.end(); ++columnName)
  {
    auto column = tws->getColumn(*columnName);
    auto columnY = column->asNumeric();
    if (!columnY) continue;
    if (columnY->getPlotRole() != DataObjects::NumericColumn::Y)
    {
      errorMessage("Column "+*columnName+" does not have the Y plot role. Skipping.");
      continue;
    }
    FunctionCurve* curve = new FunctionCurve;
    std::vector<double> x,y;
    columnX->fill(x);
    columnY->fill(y);
    curve->setData(x,y);
    curve->setWorkspace(tws);
    plot->addCurve(curve);
    plot->updateAxes();
    curve->setCurveStyle((FunctionCurve::CurveStyle)tws->getCurveStyle());
    //curve->setTitle(QString::fromStdString(tws->name() + "_" + columnXName + "_" + *columnName));
    if (yAxisTitle.empty())
    {
      yAxisTitle = *columnName;
    }
    else
    {
      yAxisTitle = "Y axis";
    }
  }
}

//------------------------------------
//   Slots
//------------------------------------

/**
 * Plot selected columns of a TbaleWorkspace
 */
void PlotTask::showTablePlot()
{
  if (!m_table) return;
  QItemSelectionModel* selection = m_table->selectionModel();
  std::string columnX,columnY,columnE;
  // column names for plotting
  std::vector<std::string> columnNames;
  // whether the columns have plot roles defined
  bool haveRoles = false;
  if (selection->hasSelection())
  {
    foreach(const QModelIndex& index,selection->selectedColumns())
    {
      auto column = m_table->getWorkspace()->getColumn(index.column());
      auto colNum = column->asNumeric();
      if (!colNum) continue;
      columnNames.push_back(column->name());
      if (colNum->getPlotRole() == DataObjects::NumericColumn::Y)
      {
        haveRoles = true;
      }
    }
    if (columnNames.empty()) return;
    if (haveRoles)
    {
      showPlot(m_table->getWorkspace(),columnNames);
    }
    else if (columnNames.size() > 1)
    {
      columnX = columnNames[0];
      columnY = columnNames[1];
      if (columnNames.size() > 2)
      {
        columnE = columnNames[2];
      }
      showPlot(m_table,columnX,columnY,columnE);
    }
  }
  //std::cerr << "Columns:\n";
  //std::for_each(columnNames.begin(),columnNames.end(),[](std::string nam){
  //  std::cerr << nam << std::endl;
  //});
}

/**
 * Set the zoomer as active picker
 */
void PlotTask::setZoomer()
{
  if (!m_plot) return;
  m_plot->enableZoomer();
}

/**
 * Set the magnidier and panner as active pickers
 */
void PlotTask::setMagnifier()
{
  if (!m_plot) return;
  m_plot->enableMagnifier();
}

void PlotTask::setCustomPicker()
{
  if (!m_plot || !m_plot->hasCustomPicker()) return;
  m_plot->enableCustomPicker();
}

void PlotTask::showPlotDialog()
{
  if (!m_plot) return;
  PlotDialog dlg(m_plot);
  dlg.exec();
}

void PlotTask::menuAboutToShow()
{
  if (m_plot)
  {
    m_setCustomPicker->setEnabled(m_plot->hasCustomPicker());
    if (m_plot->isMagnifierEnabled())
    {
      m_setMagnifier->setChecked(true);
    }
    else if (m_plot->isZoomerEnabled())
    {
      m_setZoomer->setChecked(true);
    }
    else if (m_plot->isCustomPickerEnabled())
    {
      m_setCustomPicker->setChecked(true);
    }
  }
}

} // namespace QtAPI
