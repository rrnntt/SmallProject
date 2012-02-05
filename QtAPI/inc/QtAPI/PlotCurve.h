#ifndef QTAPI_PLOTCURVE_H
#define QTAPI_PLOTCURVE_H

#include "DllExport.h"
#include "qwt_plot_curve.h"
#include <boost/shared_ptr.hpp>

namespace QtAPI
{

class QTAPI_EXPORT PlotCurve: public QObject,  public QwtPlotCurve
{
  Q_OBJECT
public:
  ~PlotCurve(){}
  static boost::shared_ptr<PlotCurve> create();
  static boost::shared_ptr<PlotCurve> create(const QString &title);
protected:
  PlotCurve();
  PlotCurve(const QString &title);
};

typedef boost::shared_ptr<PlotCurve> PlotCurve_ptr;

} // QtAPI

#endif // QTAPI_PLOTCURVE_H
