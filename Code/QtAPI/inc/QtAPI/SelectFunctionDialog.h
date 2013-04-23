#ifndef QTAPI_SELECTFUNCTIONDIALOG_H
#define QTAPI_SELECTFUNCTIONDIALOG_H

#include "QtAPI/DllExport.h"

#include <QDialog>

namespace Ui
{
  class SelectFunctionDialog;
}

namespace QtAPI
{

class QTAPI_EXPORT SelectFunctionDialog: public QDialog
{
  Q_OBJECT
public:
  struct Selection
  {
    std::string function;
    std::string formula;
  };
  SelectFunctionDialog(QWidget *parent);
  ~SelectFunctionDialog();
  Selection getSelection()const;
protected slots:
  void updateForm(int i);
protected:
  Ui::SelectFunctionDialog *m_form;
};

} // QTAPI

#endif // QTAPI_SELECTFUNCTIONDIALOG_H
