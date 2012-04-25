#include "Teach/GeometryTask.h"
#include "Teach/Graph.h"
#include "QtAPI/TaskFactory.h"
#include "QtAPI/WindowManager.h"

#include <QtGui/QMenu>

namespace Teach
{

DECLARE_TASK(GeometryTask);

GeometryTask::GeometryTask()
{
  m_newGraph = new QAction("New graph",this);
  connect(m_newGraph,SIGNAL(triggered()),this,SLOT(newGraph()));
  addAction("NewGoemetryGraph",m_newGraph);
}

QMenu* GeometryTask::menu(QtAPI::SubWindow* w) const
{
  m_menu = new QMenu("Geometry");
  m_menu->addAction(m_newGraph);
  return m_menu;
}

Graph* GeometryTask::createGraph() const
{
  auto graph = new Graph();
  QtAPI::WindowManager::instance().createSubWindow(graph);
  return graph;
}

void GeometryTask::newGraph()
{
  createGraph();
}


} // namespace Teach
