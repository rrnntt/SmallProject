#include "QtAPI/WindowManager.h"
#include "API/Framework.h"

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
    throw std::runtime_error("WindowManager does not exist");
  }
  else
  {
    return *static_cast<WindowManager*>(s);
  }
}

} // namespace QtAPI
