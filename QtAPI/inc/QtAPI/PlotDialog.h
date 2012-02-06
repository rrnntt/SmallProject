#ifndef QTAPI_PLOTDIALOG_H
#define QTAPI_PLOTDIALOG_H

#include "DllExport.h"
#include <QDialog>
#include <QPointer>

namespace Ui {
    class PlotDialog;
}

namespace QtAPI
{

class QTAPI_EXPORT Plot;

class QTAPI_EXPORT PlotDialog : public QDialog 
{
    Q_OBJECT
public:
    PlotDialog(Plot* plot);
    ~PlotDialog();

protected:
    void changeEvent(QEvent *e);
    void init();
    void initCurvePage();
    void apply();
    void applyPlot();
    void applyCurve();

private slots:

    void accept();

private:
    Ui::PlotDialog *ui;
    QPointer<Plot> m_plot;

};

} // QtAPI

#endif // QTAPI_PLOTDIALOG_H
