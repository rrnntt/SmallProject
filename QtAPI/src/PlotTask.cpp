#include "QtAPI/PlotTask.h"
#include "QtAPI/Table.h"
#include "QtAPI/Plot.h"
#include "QtAPI/CurveManager.h"
#include "QtAPI/PlotCurve.h"
#include "QtAPI/WindowManager.h"
#include "QtAPI/PlotDialog.h"

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

  m_pickerGroup = new  QActionGroup(this);
  m_pickerGroup->addAction(m_setMagnifier);
  m_pickerGroup->addAction(m_setZoomer);

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
    QMenu* theMenu = new QMenu("Plot");
    theMenu->addAction(m_showPlotDialog);
    theMenu->addSeparator();
    theMenu->addActions(m_pickerGroup->actions());
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

  PlotCurve* curve = PlotCurve::create();
  std::vector<double> x,y;
  colX->fill(x);
  colY->fill(y);
  curve->setData(&x[0],&y[0],x.size());
  auto plot = new Plot();
  plot->addCurve(curve);
  curve->setTitle(QString::fromStdString(tws->name() + "_" + columnX + "_" + columnY));
  plot->setTitle(QString::fromStdString(tws->name()));
  plot->setAxisTitle(Plot::xBottom,QString::fromStdString(columnX));
  plot->setAxisTitle(Plot::yLeft,QString::fromStdString(columnY));
  plot->setZoomBase();

  WindowManager::instance().newSubWindow(plot);
  return plot;
}

//------------------------------------
//   Slots
//------------------------------------

void PlotTask::showTablePlot()
{
  std::cerr << "showTablePlot()\n";
  if (!m_table) return;
  QItemSelectionModel* selection = m_table->selectionModel();
  std::string columnX,columnY,columnE;
  if (selection->hasSelection())
  {
    foreach(const QModelIndex& index,selection->selectedColumns())
    {
      auto column = m_table->getWorkspace()->getColumn(index.column());
      auto colNum = column->asNumeric();
      if (!colNum) continue;
      if (columnX.empty())
      {
        columnX = column->name();
      }
      else if (columnY.empty())
      {
        columnY = column->name();
      }
      else if (columnE.empty())
      {
        columnE = column->name();
      }
      else
      {
        break;
      }
    }
    if (columnY.empty()) return;
    showPlot(m_table,columnX,columnY,columnE);
  }
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

void PlotTask::showPlotDialog()
{
  if (!m_plot) return;
  PlotDialog dlg(m_plot);
  dlg.exec();
}

} // namespace QtAPI
