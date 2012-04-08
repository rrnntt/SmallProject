#ifndef QTAPI_FITWIDGET_H
#define QTAPI_FITWIDGET_H

#include "QtAPI/DllExport.h"
#include "Kernel/ParsedExpressionString.h"

#include <QWidget>

namespace Ui {
  class FitWidget;
}

namespace QtAPI
{

class QTAPI_EXPORT FitWidget: public QWidget
{
  Q_OBJECT
public:
  FitWidget(QWidget* parent);
  ~FitWidget();
protected:
  bool eventFilter(QObject *obj, QEvent *ev);
signals:
  void needUpdateExpression();
protected slots:
  void addFunction();
  void updateExpression();
private:
  Ui::FitWidget *m_form;
  Kernel::ParsedExpressionString m_expression;
};

} // QTAPI

#endif // QTAPI_FITWIDGET_H
