#ifndef GUI_PLOTCURVE_H
#define GUI_PLOTCURVE_H

#include "qwt_plot_curve.h"
#include <boost/shared_ptr.hpp>

class PlotCurve: public QObject,  public QwtPlotCurve
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

typedef boost::shared_ptr<PlotCurve> PlotCurve_sptr;

#endif // GUI_PLOTCURVE_H
