#ifndef PLOTDIALOG_H
#define PLOTDIALOG_H

#include <QDialog>

namespace Ui {
    class PlotDialog;
}

class Plot;

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

#endif // PLOTDIALOG_H
