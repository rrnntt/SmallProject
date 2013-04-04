#ifndef QTAPI_FITWIDGET_H
#define QTAPI_FITWIDGET_H

#include "QtAPI/DllExport.h"
#include "Kernel/ParsedExpressionString.h"
#include "API/WorkspaceManager.h"

#include <QWidget>

namespace Ui {
  class FitWidget;
}

namespace QtAPI
{

class QTAPI_EXPORT FitWidget: public QWidget, public Kernel::NotificationObserver
{
  Q_OBJECT
public:
  FitWidget(QWidget* parent);
  ~FitWidget();
protected:
  QString getFunction();
  bool isFunction(const std::string& fName) const;
signals:
  void needUpdateWorkspaces();
protected slots:
  void fillWorkspaces();
  void fillColumns(int i = 0);
  void fit();
private:
  void handleAdd(const API::WorkspaceManager::AddNotification& nt);
  void handleDelete(const API::WorkspaceManager::DeleteNotification& nt);

  Ui::FitWidget *m_form;
};

} // QTAPI

#endif // QTAPI_FITWIDGET_H
