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
  bool eventFilter(QObject *obj, QEvent *ev);
  QString getFunction();
  bool isFunction(const std::string& fName) const;
signals:
  void needUpdateExpression();
  void saved();
  void unsaved();
  void needUpdateWorkspaces();
protected slots:
  void addFunction();
  void updateExpression();
  void updateEditor();
  void fillWorkspaces();
  void fillColumns();
  void fit();
private:
  void handleAdd(const API::WorkspaceManager::AddNotification& nt);
  void handleDelete(const API::WorkspaceManager::DeleteNotification& nt);

  Ui::FitWidget *m_form;
  Kernel::ParsedExpressionString m_expression;
};

} // QTAPI

#endif // QTAPI_FITWIDGET_H
