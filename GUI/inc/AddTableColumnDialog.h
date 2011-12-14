#ifndef ADDTABLECOLUMNDIALOG_H
#define ADDTABLECOLUMNDIALOG_H

#include <QDialog>

namespace Ui {
    class AddTableColumnDialog;
}
class Table;

class AddTableColumnDialog : public QDialog {
    Q_OBJECT
public:
    AddTableColumnDialog(Table* table);
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
  Table* m_table;
};

#endif // ADDTABLECOLUMNDIALOG_H
