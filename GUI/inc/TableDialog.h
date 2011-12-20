#ifndef TABLEDIALOG_H
#define TABLEDIALOG_H

#include <QDialog>

namespace Ui {
    class TableDialog;
}

class TableDialog: public QDialog {
    Q_OBJECT
public:
    TableDialog(QWidget *parent);
    ~TableDialog();

    struct Data
    {
      size_t rowCount;
    };

protected:
    void changeEvent(QEvent *e);
    void init();

private:
    Ui::TableDialog *ui;

};

#endif // TABLEDIALOG_H
