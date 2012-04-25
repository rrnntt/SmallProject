#ifndef QTAPI_WINDOWMANAGER_H
#define QTAPI_WINDOWMANAGER_H

#include "QtAPI/DllExport.h"
#include "QtAPI/SubWindow.h"
#include "API/Singleton.h"

#include <QtCore/QObject>
#include <QtCore/QList>

class QMenuBar;

namespace QtAPI
{

class WindowManagerQObjectHelper;

class QTAPI_EXPORT WindowManager: protected API::Singleton
{
public:
  WindowManager();
  ~WindowManager(){}
  virtual std::string name() const {return "WindowManager";}
  static WindowManager& instance();
  static void createWindowManager(WindowManager* manager);
  /// Called by users to create a new subwindow.
  SubWindow* createSubWindow(QWidget* widget);
  virtual void customMenuBar(QMenuBar*, SubWindow* wnd) const;
  /// Get a list of all subwindows
  QList<SubWindow*> getSubWindows() const {return m_subWindows;}

protected:
  /// Implemented by concrete managers to create instances of subwindows
  virtual SubWindow* newSubWindow(QWidget* widget) = 0;
  /// Add subwindow to the list
  void addSubWindow(SubWindow* w);
  /// Remove subwindow from the list
  void removeSubWindow(SubWindow* w);

  friend class WindowManagerQObjectHelper;
  /// Helper handling signals
  WindowManagerQObjectHelper *m_objectHelper;

  /// Keep all open subwindows
  QList<SubWindow*> m_subWindows;
};

/**
 * As WindowManager shouldn't inherit from QObject to prevent multiple inheritance
 * this calss helps handle signals and slots.
 */
class WindowManagerQObjectHelper: public QObject
{
  Q_OBJECT
public:
  WindowManagerQObjectHelper(WindowManager* windowManager):
  m_windowManager(windowManager){}
public slots:
  void subWindowClosed(QtAPI::SubWindow* w);
protected:
  WindowManager* m_windowManager;
};

} // namespace QtAPI
#endif // QTAPI_WINDOWMANAGER_H
