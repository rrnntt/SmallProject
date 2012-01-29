#ifndef QTAPI_ADDTABLECOLUMNDIALOG_H
#define QTAPI_ADDTABLECOLUMNDIALOG_H

#include "QtAPI/DllExport.h"
#include <QDialog>

namespace Ui {
    class AddTableColumnDialog;
}

namespace QtAPI
{

class Table;

class QTAPI_EXPORT AddTableColumnDialog : public QDialog {
    Q_OBJECT
public:
    AddTableColumnDialog(QWidget* parent);
    ~AddTableColumnDialog();
    std::string getType() const;
    std::string getName() const;

protected:
  void init();

private slots:
  void changeEvent(QEvent *e);
  //void accept();
  //void apply();

private:
  Ui::AddTableColumnDialog *ui;
};

} // QtAPI

#endif // QTAPI_ADDTABLECOLUMNDIALOG_H
