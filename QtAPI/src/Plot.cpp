#include "QtAPI/Plot.h"
#include "QtAPI/PlotDefaults.h"
#include "QtAPI/PlotCurve.h"
#include "QtAPI/CurveManager.h"
#include "QtAPI/PlotPicker.h"

#include "qwt_plot_canvas.h"
#include "qwt_plot_layout.h"
#include "qwt_scale_draw.h"
#include "qwt_text.h"
#include "qwt_legend.h"
#include "qwt_plot_zoomer.h"
#include "qwt_plot_panner.h"
#include "qwt_plot_magnifier.h"

#include <QPalette>
#include <iostream>
#include <algorithm>

//using namespace Formula;

namespace QtAPI
{

Plot::Plot(QWidget *parent)
:QwtPlot(parent),
m_curve_count(),
m_customPicker(nullptr)
{
  init();
}

void Plot::init()
{
  setAutoDelete(false);
  //canvas()->setFrameShadow(QFrame::Plain);
  canvas()->setFrameShape(QFrame::NoFrame);
  setAutoFillBackground(true);
  QPalette palette;
  palette.setColor(QPalette::Window,Qt::white);
  setPalette(palette);
  plotLayout()->setAlignCanvasToScales(true);
  //enableAxis(QwtPlot::yLeft,false);

  QwtText axisText("X axis");
  axisText.setFont(PlotDefaults::instance().axisFont());
  setAxisTitle(QwtPlot::xBottom,axisText);

  axisText.setText("Y axis");
  setAxisTitle(QwtPlot::yLeft,axisText);

  QwtText titleText("Title");
  titleText.setFont(PlotDefaults::instance().titleFont());
  titleText.setColor(PlotDefaults::instance().titleColor());
  setTitle(titleText);

  // zoomer
  m_zoomer = new QwtPlotZoomer(QwtPlot::xBottom, QwtPlot::yLeft,
      QwtPicker::DragSelection | QwtPicker::CornerToCorner, QwtPicker::AlwaysOff, canvas());
  m_zoomer->setRubberBandPen(QPen(Qt::black));

  // panner and magnifier
  m_magnifier = new QwtPlotMagnifier(canvas());
  m_magnifier->setAxisEnabled(QwtPlot::yRight,false);
  m_magnifier->setZoomInKey(Qt::Key_Plus, Qt::ShiftModifier);
  m_panner = new QwtPlotPanner(canvas());
  m_panner->setAxisEnabled(QwtPlot::yRight,false);

  // set default picker
  enableMagnifier();
  //insertLegend(new QwtLegend(),QwtPlot::RightLegend); // to create legend
  //insertLegend(NULL,QwtPlot::RightLegend); // to delete legend
}

void Plot::addCurve(PlotCurve* curve)
{
  curve->attach(this);
  CurveManager::instance().addCurve(curve);
}

QList<PlotCurve*> Plot::getCurves() const
{
  QList<PlotCurve*> out;
  QwtPlotItemList items = itemList();
  foreach(QwtPlotItem* item,items)
  {
    auto curve = dynamic_cast<PlotCurve*>(item);
    out.append(curve);
  }
  return out;
}

QStringList Plot::getCurveNames() const
{
  QList<PlotCurve*> curves = getCurves();
  QStringList names;
  foreach(const PlotCurve* curve,curves)
  {
    QString title = curve->title().text();
    if (title.isEmpty())
    {
      title = "Unknown";
    }
    names << title;
  }
  return names;
}

PlotCurve* Plot::getCurve(QString name)const
{
  auto curves = getCurves();
  foreach(PlotCurve* curve,curves)
  {
    if (curve->title() == name)
    {
      return curve;
    }
  }
  return nullptr;
}

/**
 * Set current scale to be zoomer's base
 */
void Plot::setZoomBase()
{
  m_zoomer->setZoomBase();
}

bool Plot::isZoomerEnabled() const
{
  return m_zoomer->isEnabled();
}

void Plot::enableZoomer()
{
  disablePickers();
  m_zoomer->setEnabled(true);
  canvas()->setCursor(Qt::CrossCursor);
}

bool Plot::isMagnifierEnabled() const
{
  return m_magnifier->isEnabled();
}

void Plot::enableMagnifier()
{
  disablePickers();
  m_magnifier->setEnabled(true);
  m_panner->setEnabled(true);
  canvas()->setCursor(Qt::OpenHandCursor);
}

void Plot::disablePickers()
{
  m_zoomer->setEnabled(false);
  m_magnifier->setEnabled(false);
  m_panner->setEnabled(false);
  if (m_customPicker)
  {
    m_customPicker->setEnabled(false);
  }
}

void Plot::enableCustomPicker()
{
  if (! m_customPicker) return;
  disablePickers();
  m_customPicker->setEnabled(true);
  canvas()->setCursor(Qt::ArrowCursor);
}

void Plot::setCustomPicker(PlotPicker* picker)
{
  if (m_customPicker)
  {
    m_customPicker->setEnabled(false);
    delete m_customPicker;
  }
  m_customPicker = picker;
  enableCustomPicker();
}

bool Plot::hasCustomPicker() const
{
  return m_customPicker != nullptr;
}

bool Plot::isCustomPickerEnabled() const
{
  return m_customPicker && m_customPicker->isEnabled();
}

double Plot::getXStart() const
{
  return m_plot->canvasMap(QtAPI::Plot::xBottom).s1();
}

double Plot::getXEnd() const
{
  return m_plot->canvasMap(QtAPI::Plot::xBottom).s2();
}

double Plot::getYStart() const
{
  return m_plot->canvasMap(QtAPI::Plot::yLeft).s1();
}

double Plot::getYEnd() const
{
  return m_plot->canvasMap(QtAPI::Plot::yLeft).s2();
}

} // QtAPI
