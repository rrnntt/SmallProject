#ifndef QTAPI_SUBWINDOW_H
#define QTAPI_SUBWINDOW_H

#include "DllExport.h"

#include <QtGui/QMdiSubWindow>

class QMenu;

namespace QtAPI
{

class QTAPI_EXPORT SubWindow: public QMdiSubWindow
{
  Q_OBJECT
public:
  SubWindow(QWidget *parent = NULL);
  virtual QMenu* makeContext() const {return nullptr;}
  QWidget* getWidget() const {return m_internalWidget;}
  /// Get the unique id
  size_t getID() const {return m_id;}
signals:
  void subWindowBecameActive(QtAPI::SubWindow* w);
  void subWindowClosed(QtAPI::SubWindow* w);
protected slots:
  void processStateChanged(Qt::WindowStates oldState, Qt::WindowStates newSTate);
protected:
  bool event(QEvent * e);
  void closeEvent(QCloseEvent * e);
  /// Set user defined internal child widget
  void setInternalWidget(QWidget* w) {m_internalWidget = w;}
  /// Set id
  void setID(size_t id){ m_id = id; }
  /// Internal widget of user defined type
  QWidget *m_internalWidget;
  /// unique id
  size_t m_id;

  friend class WindowManager;
};

} // QtAPI

#endif // QTAPI_SUBWINDOW_H
