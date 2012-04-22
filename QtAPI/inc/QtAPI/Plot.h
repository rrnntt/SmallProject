#ifndef QTAPI_PLOT_H
#define QTAPI_PLOT_H

#include "DllExport.h"
#include "QtAPI/PlotWorkspace.h"

#include "qwt_plot.h"

#include <boost/shared_ptr.hpp>

class QwtPlotZoomer;
class QwtPlotMagnifier;
class QwtPlotPanner;

namespace QtAPI
{

class PlotCurve;
class PlotPicker;
class PlotRescaler;

class QTAPI_EXPORT Plot: public QwtPlot
{
  Q_OBJECT
public:
  Plot(QWidget *parent = NULL);
  ~Plot();
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
  bool hasExternalPicker() const;
  void setExternalPicker(PlotPicker* picker);

  double getXStart() const;
  double getXEnd() const;
  double getYStart() const;
  double getYEnd() const;

  void setRescaler(PlotRescaler* rescaler);

  void delayedReplot();

signals:
  void needReplot();
protected:
  void init();
  void resizeEvent(QResizeEvent* e);
  void paintEvent(QPaintEvent*);

private:
  int m_curve_count;
  QList<QColor> m_colors; ///< colors for stored curves
  int m_colorIndex;

  QwtPlotZoomer* m_zoomer; ///< does zooming
  QwtPlotPanner* m_panner; ///< does panning
  QwtPlotMagnifier* m_magnifier; ///< does magnifying
  PlotPicker* m_customPicker;
  mutable PlotPicker* m_externalPicker;

  PlotRescaler* m_rescaler; ///< helps to rescale the axes after resizing the canvas.
  bool m_painting;  ///< if plot in a QPaint event

  /// Store the plot objects 
  PlotWorkspace_sptr m_plotObjects;

};

} // QtAPI

#endif // QTAPI_PLOT_H
