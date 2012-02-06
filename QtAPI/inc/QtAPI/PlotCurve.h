#ifndef QTAPI_PLOTCURVE_H
#define QTAPI_PLOTCURVE_H

#include "DllExport.h"
#include "qwt_plot_curve.h"

#include <QPointer>

namespace QtAPI
{

class QTAPI_EXPORT PlotCurve: public QObject,  public QwtPlotCurve
{
  Q_OBJECT
public:
  ~PlotCurve();
  static QPointer<PlotCurve> create();
  static QPointer<PlotCurve> create(const QString &title);
protected:
  PlotCurve();
  PlotCurve(const QString &title);
};

} // QtAPI

#endif // QTAPI_PLOTCURVE_H
