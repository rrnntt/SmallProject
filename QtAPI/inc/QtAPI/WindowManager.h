#ifndef QTAPI_WINDOWMANAGER_H
#define QTAPI_WINDOWMANAGER_H

#include "QtAPI/DllExport.h"
#include "QtAPI/SubWindow.h"
#include "API/Singleton.h"

class QMenuBar;

namespace QtAPI
{

class QTAPI_EXPORT WindowManager: protected API::Singleton
{
public:
  WindowManager();
  ~WindowManager(){}
  virtual std::string name() const {return "WindowManager";}
  static WindowManager& instance();
  static void createWindowManager(WindowManager* manager);
  virtual SubWindow* newSubWindow(QWidget* widget) = 0;
  virtual void customMenuBar(QMenuBar*, SubWindow* wnd) const;
};

} // namespace QtAPI
#endif // QTAPI_WINDOWMANAGER_H
