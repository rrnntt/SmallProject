#ifndef QTAPI_PLOTDIALOG_H
#define QTAPI_PLOTDIALOG_H

#include "DllExport.h"
#include <QDialog>
#include <QPointer>

namespace Ui {
    class PlotDialog;
}

class QListWidget;

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
    void initCurveCopyPage();
    void apply();
    void applyPlot();
    void applyCurve();
    /// Fills a list widget with curves from a plot
    void fillCurves(QListWidget* cb, Plot* plot);

private slots:

    void accept();
    void updateCurveList(int);
    void copyCurve();
    void deleteCurve();
    void updateCurve(int);
    void updateThisCurveList();

private:
    Ui::PlotDialog *ui;
    QPointer<Plot> m_plot;

};

} // QtAPI

#endif // QTAPI_PLOTDIALOG_H
