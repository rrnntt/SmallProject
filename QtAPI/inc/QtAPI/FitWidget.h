#ifndef QTAPI_FITWIDGET_H
#define QTAPI_FITWIDGET_H

#include "QtAPI/DllExport.h"
#include "Kernel/ParsedExpressionString.h"
#include "API/WorkspaceManager.h"

#include <QWidget>

class QAction;

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
  QString getWorkspaceName() const;
  QString getXColumn() const;
  QString getYColumn() const;
protected:
  QString getFunction();
  bool isFunction(const std::string& fName) const;
  void initPlot();
signals:
  void needUpdateWorkspaces();
protected slots:
  void fillWorkspaces();
  void fillColumns(int i = 0);
  void fit();
  void replot(int);
  void replot(){ replot(0); }
  void showPlotDialog();
private:
  void handleAdd(const API::WorkspaceManager::AddNotification& nt);
  void handleDelete(const API::WorkspaceManager::DeleteNotification& nt);

  Ui::FitWidget *m_form;
  QAction *m_showPlotDialog;
  QAction *m_replot;
};

} // QTAPI

#endif // QTAPI_FITWIDGET_H
