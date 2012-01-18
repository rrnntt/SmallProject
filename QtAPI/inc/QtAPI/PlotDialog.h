#ifndef QTAPI_PLOTDIALOG_H
#define QTAPI_PLOTDIALOG_H

#include "DllExport.h"
#include <QDialog>

namespace Ui {
    class PlotDialog;
}

namespace QtAPI
{

class QTAPI_EXPORT Plot;

class PlotDialog : public QDialog {
    Q_OBJECT
public:
    PlotDialog(QWidget *parent, Plot* plot);
    ~PlotDialog();

protected:
    void changeEvent(QEvent *e);
    void init();
    void apply();

private slots:

    void accept();

private:
    Ui::PlotDialog *ui;
    Plot* m_plot;

};

} // QtAPI

#endif // QTAPI_PLOTDIALOG_H
