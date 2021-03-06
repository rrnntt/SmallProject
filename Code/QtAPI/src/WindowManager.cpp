#include "QtAPI/WindowManager.h"
#include "QtAPI/TaskManager.h"
#include "Kernel/Framework.h"

#include <QtGui/QMenuBar>
#include <QtCore/QMap>

namespace QtAPI
{

WindowManager::WindowManager():
m_objectHelper(new WindowManagerQObjectHelper(this)),
m_idSeed(1)
{
}

/**
 * Set a pointer to the concrete window manager
 */
void WindowManager::createWindowManager(WindowManager* manager)
{
  Kernel::Framework::instance().registerSingleton("WindowManager",manager);
}

WindowManager& WindowManager::instance()
{
  Kernel::Singleton* s = Kernel::Framework::instance().getSingleton("WindowManager");
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

/**
 * Creates a new sub-window. Calls virtual newSubWindow() method which must be implemented
 * by concrete window managers.
 * @param widget :: User provided internal widget of the subwindow.
 */
SubWindow* WindowManager::createSubWindow(QWidget* widget)
{
  SubWindow* sw = this->newSubWindow(widget);
  if ( widget->parentWidget() != sw )
  {
    throw std::runtime_error("Wrongly set parent");
  }
  sw->setInternalWidget(widget);
  sw->setID(m_idSeed++);
  addSubWindow(sw);
  QObject::connect(sw,SIGNAL(subWindowClosed(QtAPI::SubWindow*)),m_objectHelper,SLOT(subWindowClosed(QtAPI::SubWindow*)));
  return sw;
}

/// Add subwindow to the list
void WindowManager::addSubWindow(SubWindow* w)
{
  m_subWindows << w;
}

/// Remove subwindow to the list
void WindowManager::removeSubWindow(SubWindow* w)
{
  m_subWindows.removeOne(w);
}

/// Get a subwindow by its id
SubWindow* WindowManager::getSubWindow(size_t id) const
{
  for(auto sw = m_subWindows.begin(); sw != m_subWindows.end(); ++sw)
  {
    if ( (**sw).getID() == id ) return *sw;
  }
  return nullptr;
}

//-------------------------------------------------------
//  WindowManagerQObjectHelper methods
//-------------------------------------------------------

void WindowManagerQObjectHelper::subWindowClosed(SubWindow* w)
{
  //std::cerr << "Subwindow closed" << std::endl;
  m_windowManager->removeSubWindow(w);
}


} // namespace QtAPI
