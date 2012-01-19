#include "QtAPI/WindowManager.h"
#include "QtAPI/TaskManager.h"
#include "API/Framework.h"

#include <QtGui/QMenuBar>

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

void WindowManager::customMenuBar(QMenuBar* menubar, SubWindow* wnd) const
{
  TaskManager& manager = TaskManager::instance();
  auto taskNames = manager.getObjectNames();
  menubar->clear();
  for(auto name = taskNames.begin(); name != taskNames.end();++name)
  {
    menubar->addMenu(manager.retrieve(*name)->menu(wnd));
  };
}

} // namespace QtAPI
