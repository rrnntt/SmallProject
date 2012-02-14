#include "Goblin/GoblinTask.h"
#include "Goblin/EnergyList.h"
#include "Goblin/LineList.h"
#include "Goblin/ComDiffDialog.h"
#include "Goblin/GoblinPlotPicker.h"

#include "QtAPI/SubWindow.h"
#include "QtAPI/Table.h"
#include "QtAPI/WindowManager.h"
#include "QtAPI/TaskFactory.h"
#include "QtAPI/TaskManager.h"
#include "QtAPI/PlotTask.h"

#include "API/WorkspaceFactory.h"

#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QtGui/QFileDialog>
#include <QtCore/QFileInfo>

namespace Goblin
{

DECLARE_TASK(GoblinTask);

GoblinTask::GoblinTask()
{
  m_loadEnergyList = new QAction("Load energy list",this);
  connect(m_loadEnergyList,SIGNAL(triggered()),this,SLOT(loadEnergyList()));
  addAction("LoadEnergyList",m_loadEnergyList);

  m_loadLineList = new QAction("Load line list",this);
  connect(m_loadLineList,SIGNAL(triggered()),this,SLOT(loadLineList()));
  addAction("LoadLineList",m_loadLineList);

  m_comdiff = new QAction("comdiff",this);
  connect(m_comdiff,SIGNAL(triggered()),this,SLOT(comDiff()));
  addAction("ComDiff",m_comdiff);

  m_plotLineList = new QAction("Plot",this);
  connect(m_plotLineList,SIGNAL(triggered()),this,SLOT(plotLineList()));
  addAction("PlotLineList",m_plotLineList);
}

QMenu* GoblinTask::menu(QtAPI::SubWindow* w) const
{
  QMenu* menu = new QMenu("Goblin");
  menu->addAction(m_loadLineList);
  menu->addAction(m_loadEnergyList);
  menu->addAction(m_comdiff);
  m_table = nullptr;
  if (w)
  {
    QtAPI::Table* table = qobject_cast<QtAPI::Table*>(w->widget());
    if (table)
    {
      addTableActions(menu,table);
    }
  }
  return menu;
}

void GoblinTask::addTableActions(QMenu* menu, QtAPI::Table* table) const
{
  m_table = table;
  auto lineList = boost::dynamic_pointer_cast<LineList>(table->getWorkspace());
  if (lineList)
  {
    menu->addAction(m_plotLineList);
  }
  auto enList = boost::dynamic_pointer_cast<EnergyList>(table->getWorkspace());
  if (enList)
  {
  }
}

void GoblinTask::loadEnergyList()
{
  QString fileName = QFileDialog::getOpenFileName(nullptr,"Open an energy list");
  if (!fileName.isEmpty())
  {
    try
    {
      QFileInfo file(fileName);
      QString name = file.baseName();
      DataObjects::TableWorkspace_ptr ws( dynamic_cast<DataObjects::TableWorkspace*>(
        API::WorkspaceFactory::instance().create("EnergyList")  ));
      ws->loadAscii(fileName.toStdString());
      API::WorkspaceManager::instance().addOrReplace(name.toStdString(),ws);
      QtAPI::SubWindow* wnd = QtAPI::WindowManager::instance().newSubWindow(new QtAPI::Table(ws));
      wnd->setWindowTitle(name);
    }
    catch(std::exception& e)
    {
      errorMessage(std::string("Loading failed:\n")+e.what());
    }
  }
}

void GoblinTask::loadLineList()
{
  QString fileName = QFileDialog::getOpenFileName(nullptr,"Open a linelist");
  if (!fileName.isEmpty())
  {
    try
    {
      QFileInfo file(fileName);
      QString name = file.baseName();
      DataObjects::TableWorkspace_ptr ws( dynamic_cast<DataObjects::TableWorkspace*>(
        API::WorkspaceFactory::instance().create("LineList")  ));
      ws->loadAscii(fileName.toStdString());
      API::WorkspaceManager::instance().addOrReplace(name.toStdString(),ws);
      QtAPI::SubWindow* wnd = QtAPI::WindowManager::instance().newSubWindow(new QtAPI::Table(ws));
      wnd->setWindowTitle(name);
    }
    catch(std::exception& e)
    {
      errorMessage(std::string("Loading failed:\n")+e.what());
    }
  }
}

void GoblinTask::comDiff()
{
  ComDiffDialog *dlg = new ComDiffDialog(NULL);
  dlg->setAttribute(Qt::WA_DeleteOnClose);
  dlg->show();
}

void GoblinTask::createEnergyList()
{
  std::string name = getInput("Enter name for new EnergyList");
  if (!name.empty())
  {
    try
    {
      DataObjects::TableWorkspace_ptr ws( dynamic_cast<DataObjects::TableWorkspace*>(
        API::WorkspaceFactory::instance().create("EnergyList")  ));
      API::WorkspaceManager::instance().addOrReplace(name,ws);
      QtAPI::SubWindow* wnd = QtAPI::WindowManager::instance().newSubWindow(new QtAPI::Table(ws));
      wnd->setWindowTitle(QString::fromStdString(name));
    }
    catch(std::exception& e)
    {
      errorMessage(std::string("Creating EnergyList failed:\n")+e.what());
    }
  }
}

void GoblinTask::plotLineList()
{
  if (!m_table) return;
  auto lineList = boost::dynamic_pointer_cast<LineList>(m_table->getWorkspace());
  if (!lineList) return;
  auto task = QtAPI::TaskManager::instance().getAs<QtAPI::PlotTask>("PlotTask");
  if (task)
  {
    auto plot = task->showPlot(lineList,std::vector<std::string>(1,"Intensity"));
    plot->setCustomPicker(new GoblinPlotPicker(plot));
  }
}

} // namespace Goblin
