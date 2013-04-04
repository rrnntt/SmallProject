#include "QtAPI/TableTask.h"
#include "QtAPI/Table.h"
#include "QtAPI/SubWindow.h"
#include "QtAPI/WindowManager.h"
#include "QtAPI/FunctionCurve.h"

#include "API/WorkspaceFactory.h"
#include "API/AlgorithmFactory.h"

#include "Numeric/ChebfunWorkspace.h"
#include "Numeric/ChebfunVector.h"

#include <QtGui/QAction>
#include <QtGui/QFileDialog>
#include <QtCore/QFileInfo>

namespace QtAPI
{

TableTask::TableTask()
{
  m_showTableDialog = new QAction("Set values...",this);
  connect(m_showTableDialog,SIGNAL(triggered()),this,SLOT(showTableDialog()));
  addAction("SetValues",m_showTableDialog);
}

QMenu* TableTask::menu(SubWindow* w) const
{
  if (!w) return nullptr;
  Table* table = qobject_cast<Table*>(w->widget());
  if (!table) return nullptr;
  m_table = table;
  QMenu* theMenu = new QMenu("Table");
  theMenu->addAction(m_showTableDialog);
  return theMenu;
}

void TableTask::showTableDialog()
{
  if (!m_table) return;
  m_table->showTableDialog();
}

QPointer<Table> TableTask::newTable() const
{
  API::TableWorkspace_ptr ws( dynamic_cast<API::TableWorkspace*>(
    API::WorkspaceFactory  ::instance().create("TableWorkspace")
    ));
  QString name = "Table_" + QString::number(API::WorkspaceManager::instance().size() + 1);// TODO: not robust
  API::WorkspaceManager::instance().addOrReplace(name.toStdString(),ws);
  ws->addColumn("double","X");
  ws->addColumn("double","Y");
  ws->setRowCount(30);
  Table* table = new Table(ws);
  SubWindow* wnd = WindowManager::instance().createSubWindow(table);
  wnd->setWindowTitle(name);
  return table;
}

/**
 * Load a TableWorkspace from an ascii file and create a Table for it.
 */
void TableTask::loadAsciiTable()const
{
  QString fileName = QFileDialog::getOpenFileName(nullptr,"Open a table");
  if (!fileName.isEmpty())
  {
    try
    {
      QFileInfo file(fileName);
      QString name = file.baseName();
      API::TableWorkspace_ptr ws( dynamic_cast<API::TableWorkspace*>(
        API::WorkspaceFactory::instance().create("TableWorkspace")  ));
      ws->loadAscii(fileName.toStdString());
      API::WorkspaceManager::instance().addOrReplace(name.toStdString(),ws);
      SubWindow* wnd = WindowManager::instance().createSubWindow(new Table(ws));
      wnd->setWindowTitle(name);
    }
    catch(std::exception& e)
    {
      errorMessage(std::string("Loading failed:\n")+e.what());
    }
  }
}

/**
 * Open a Table sub-window for a table workspace.
 * @param tws :: Shared pointer to a TableWorkspace to be displayed in a Table.
 */
void TableTask::showTable(boost::shared_ptr<API::TableWorkspace> tws)
{
  SubWindow* wnd = WindowManager::instance().createSubWindow(new Table(tws));
  wnd->setWindowTitle(QString::fromStdString(tws->name()));
}

/**
 * Open a Table sub-window for a table workspace.
 * @param wsName :: A name of a TableWorkspace to be displayed in a Table.
 */
void TableTask::showTable(const QString& wsName)
{
  try
  {
    auto ws = API::WorkspaceManager::instance().retrieve(wsName.toStdString());
    auto tws = boost::dynamic_pointer_cast<API::TableWorkspace>( ws );
    if (tws)
    {
      showTable(tws);
    }
    else if ( boost::dynamic_pointer_cast<Numeric::ChebfunWorkspace>( ws ) ||
      boost::dynamic_pointer_cast<Numeric::ChebfunVector>( ws ))
    {
      auto alg = API::AlgorithmFactory::instance().createAlgorithm("ChebfunToTable");
      if ( !alg )
      {
        errorMessage("Cannot display workspace " + ws->API::Workspace::name() );
        return;
      }
      alg->setClassProperty("InputWorkspace",ws);
      alg->execute();
      tws = alg->getClass("OutputWorkspace");
      if ( tws )
      {
        showTable( tws );
      }
    }
    else
    {
      errorMessage("Workspace " + wsName.toStdString()+" is not a TableWorkspace.");
      return;
    }
  }
  catch(std::exception& e)
  {
    errorMessage("Cannot find workspace "+wsName.toStdString()+": \n"+e.what());
    }
}

} // namespace QtAPI
