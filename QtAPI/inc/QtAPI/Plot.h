#ifndef QTAPI_PLOT_H
#define QTAPI_PLOT_H

#include "DllExport.h"
#include "qwt_plot.h"
#include <boost/shared_ptr.hpp>

class QwtPlotZoomer;
class QwtPlotMagnifier;
class QwtPlotPanner;

namespace QtAPI
{

class PlotCurve;
class PlotPicker;

class QTAPI_EXPORT Plot: public QwtPlot
{
  Q_OBJECT
public:
  Plot(QWidget *parent = NULL);
  void addCurve(PlotCurve* curve);
  QList<PlotCurve*> getCurves() const;
  QStringList getCurveNames() const;
  PlotCurve* getCurve(QString name)const;
  PlotCurve* bringForwardCurve(size_t i);

  void setZoomBase();
  bool isZoomerEnabled() const;
  void enableZoomer();
  bool isMagnifierEnabled() const;
  void enableMagnifier();
  void disablePickers();
  void enableCustomPicker();
  bool hasCustomPicker() const;
  bool isCustomPickerEnabled() const;
  void setCustomPicker(PlotPicker* picker);

  double getXStart() const;
  double getXEnd() const;
  double getYStart() const;
  double getYEnd() const;
protected:
  void init();
private:
  int m_curve_count;
  QwtPlotZoomer* m_zoomer; ///< does zooming
  QwtPlotPanner* m_panner; ///< does panning
  QwtPlotMagnifier* m_magnifier; ///< does magnifying
  PlotPicker* m_customPicker;
  QList<QColor> m_colors; ///< colors for stored curves
  int m_colorIndex;
};

} // QtAPI

#endif // QTAPI_PLOT_H
