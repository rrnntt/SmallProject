#include "QtAPI/WindowManager.h"
#include "QtAPI/TaskManager.h"
#include "API/Framework.h"

#include <QtGui/QMenuBar>
#include <QtCore/QMap>

namespace QtAPI
{

WindowManager::WindowManager()
{
}

/**
 * Set a pointer to the concrete window manager
 */
void WindowManager::createWindowManager(WindowManager* manager)
{
  API::Framework::instance().registerSingleton("WindowManager",manager);
}

WindowManager& WindowManager::instance()
{
  Singleton* s = API::Framework::instance().getSingleton("WindowManager");
  if (s == nullptr)
  {
    // call createWindowManager(...) first
    throw std::runtime_error("WindowManager does not exist");
  }
  else
  {
    return *static_cast<WindowManager*>(s);
  }
}

/**
 * Custimize a menubar for an active window.
 * @param menubar :: The menubar to customize.
 * @param wnd :: A Pointer to current sub-window
 */
void WindowManager::customMenuBar(QMenuBar* menubar, SubWindow* wnd) const
{
  TaskManager& manager = TaskManager::instance();
  auto taskNames = manager.getObjectNames();
  menubar->clear();
  QMultiMap<int,Task_ptr> orderedTasks;
  // loop the tasks and add their menus to the menubar
  for(auto name = taskNames.begin(); name != taskNames.end();++name)
  {
    Task_ptr task = manager.retrieve(*name);
    if (task)
    {
      orderedTasks.insert(task->menuOrder(), task);
    }
  };
  for(auto task = orderedTasks.begin(); task != orderedTasks.end(); ++task)
  {
    QMenu* menu = task.value()->menu(wnd);
    if (menu)
    {
      menubar->addMenu(menu);
    }
  }
}

} // namespace QtAPI
