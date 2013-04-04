#include "QtAPI/Plot.h"
#include "QtAPI/PlotDefaults.h"
#include "QtAPI/PlotCurve.h"
#include "QtAPI/CurveManager.h"
#include "QtAPI/PlotPicker.h"
#include "QtAPI/PlotRescaler.h"
#include "QtAPI/PlotItem.h"
#include "QtAPI/FunctionCurve.h"

#include "qwt_plot_canvas.h"
#include "qwt_plot_layout.h"
#include "qwt_scale_draw.h"
#include "qwt_text.h"
#include "qwt_legend.h"
#include "qwt_plot_zoomer.h"
#include "qwt_plot_panner.h"
#include "qwt_plot_magnifier.h"

#include <QPalette>
#include <QResizeEvent>
#include <QPixmap>
#include <QPrinter>

#include <iostream>
#include <algorithm>

//using namespace Formula;

namespace QtAPI
{

Plot::Plot(QWidget *parent)
:QwtPlot(parent),
//m_curve_count(),
m_customPicker(nullptr),
m_externalPicker(nullptr),
m_rescaler(nullptr),
m_painting(false),
m_curves(new PlotWorkspace),
m_plotObjects(new PlotWorkspace)
{
  init();
  //PlotItem* item = new PlotItem(m_plotObjects, 0);
  //item->attach(this);
}

Plot::~Plot()
{
  if (m_rescaler)
  {
    delete m_rescaler;
  }
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
  m_colors << Qt::black << Qt::blue << Qt::green << Qt::red << Qt::cyan << Qt::magenta << Qt::yellow << Qt::gray;
  m_colors << Qt::darkBlue << Qt::darkGreen << Qt::darkRed << Qt::darkCyan << Qt::darkMagenta << Qt::darkYellow << Qt::darkGray;
  m_colorIndex = 0;

  connect(this,SIGNAL(needReplot()),this,SLOT(replot()),Qt::QueuedConnection);
}

void Plot::delayedReplot()
{
  emit needReplot();
}

void Plot::setRescaler(PlotRescaler* rescaler)
{
  if (m_rescaler) delete m_rescaler;
  m_rescaler = rescaler;
}

void Plot::resizeEvent(QResizeEvent* e)
{
  QwtPlot::resizeEvent(e);
  if (m_rescaler && !m_painting)
  {
    m_rescaler->rescale(e->oldSize(),e->size());
    delayedReplot();
  }
}

void Plot::paintEvent(QPaintEvent* e)
{
  m_painting = true;
  QwtPlot::paintEvent(e);
  m_painting = false;
}

//QList<PlotCurve*> Plot::getCurves() const
//{
//  QList<PlotCurve*> out;
//  QwtPlotItemList items = itemList();
//  foreach(QwtPlotItem* item,items)
//  {
//    auto curve = dynamic_cast<PlotCurve*>(item);
//    out.append(curve);
//  }
//  return out;
//}
//
//QStringList Plot::getCurveNames() const
//{
//  QList<PlotCurve*> curves = getCurves();
//  QStringList names;
//  foreach(const PlotCurve* curve,curves)
//  {
//    QString title = curve->title().text();
//    if (title.isEmpty())
//    {
//      title = "Unknown";
//    }
//    names << title;
//  }
//  return names;
//}

//PlotCurve* Plot::getCurve(QString name)const
//{
//  auto curves = getCurves();
//  foreach(PlotCurve* curve,curves)
//  {
//    if (curve->title() == name)
//    {
//      return curve;
//    }
//  }
//  return nullptr;
//}
//
//PlotCurve* Plot::bringForwardCurve(size_t i)
//{
//  auto curves = getCurves();
//  if (i < curves.size())
//  {
//    auto curve = curves[i];
//    curve->detach();
//    curve->attach(this);
//    return curve;
//  }
//  return nullptr;
//}

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
  if (m_externalPicker)
  {
    m_externalPicker->setEnabled(false);
    delete m_externalPicker;
    m_externalPicker = nullptr;
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

void Plot::setExternalPicker(PlotPicker* picker)
{
  disablePickers();
  m_externalPicker = picker;
  picker->setEnabled(true);
}

bool Plot::hasExternalPicker() const
{
  if (m_externalPicker != nullptr && !m_externalPicker->isEnabled())
  {
    delete m_externalPicker;
    m_externalPicker = nullptr;
  }
  return m_externalPicker != nullptr;
}

double Plot::getXStart() const
{
  return canvasMap(QtAPI::Plot::xBottom).s1();
}

double Plot::getXEnd() const
{
  return canvasMap(QtAPI::Plot::xBottom).s2();
}

double Plot::getYStart() const
{
  return canvasMap(QtAPI::Plot::yLeft).s1();
}

double Plot::getYEnd() const
{
  return canvasMap(QtAPI::Plot::yLeft).s2();
}

/* PlotObjects */

/**
 * Add a new object to the PlotWorkspace. It will be displayed.
 * @param obj :: Pointer to the new object.
 */
void Plot::addObject(PlotObject* obj)
{
  auto id = m_plotObjects->addObject(obj);
  addItem( new PlotItem(m_plotObjects, id) );
}

/**
 * Add a new item to the Plot to be displayed.
 * @param item :: Pointer to the new item. It may refer to object in other workspaces.
 */
void Plot::addItem(PlotItem* item)
{
  item->attach(this);
}

/**
 * Add a curve. Automatically set line colour.
 */
void Plot::addCurve(FunctionCurve* curve)
{
  auto id = m_curves->addObject(curve);
  addItem( new PlotItem(m_curves, id) );
  curve->setPen(QPen(m_colors[m_colorIndex]));
  ++m_colorIndex;
  m_colorIndex %= m_colors.size();
}

/// Return a list of curve ids
QList<PlotObject::id_t> Plot::getCurveIDs() const
{
  return m_curves->getIDs();
}

/**
 * Return a list of curve items. Only FunctionCurve object are included.
 */
QList<PlotItem*> Plot::getAllCurveItems() const
{
  auto items = this->itemList();
  QList<PlotItem*> out;
  foreach(QwtPlotItem* item, items)
  {
    auto plotItem = dynamic_cast<PlotItem*>(item);
    if ( plotItem && plotItem->getWorkspace() )
    {
      auto obj = plotItem->getWorkspace()->getPlotObject(plotItem->getID());
      if ( obj && dynamic_cast<FunctionCurve*>(obj) )
      {
        out << plotItem;
      }
    }
  }
  return out;
}

/**
 * Return a pointer to a curve by an id. DO NOT KEEP THE POINTER, KEEP ID!
 * If curve doesn't exist return nullptr.
 * @param id :: Id of a curve.
 */
FunctionCurve* Plot::getCurve(PlotObject::id_t id) const
{
  auto obj = m_curves->getPlotObject(id);
  if ( !obj ) return nullptr;
  return dynamic_cast<FunctionCurve*>(obj);
}

/**
 * Bring forward the next curve. 
 */
PlotObject::id_t Plot::bringForwardNextCurve(PlotObject::id_t id)
{
  // find ID that follows id in m_curves workspace
  auto ids = m_curves->getIDs();
  for(int i = 0; i < ids.size(); ++i)
  {
    if ( ids[i] == id )
    {
      if ( i < ids.size() - 1 ) id = ids[i + 1];
      else
        id = ids[0];
    }
  }

  // find the item with that id
  auto items = this->itemList();
  foreach(QwtPlotItem* item, items)
  {
    auto plotItem = dynamic_cast<PlotItem*>(item);
    if ( plotItem && plotItem->getID() == id )
    {// detach and re-attach it
      plotItem->detach();
      plotItem->attach(this);
    }
  }

  return id;
}

/**
 * Creates a plot item pointing to an object owned by this plot.
 * @param id :: ID of the object.
 */
PlotItem* Plot::createPlotItem(PlotObject::id_t id) const
{
  auto obj = m_curves->getPlotObject(id);
  if ( obj )
  {
    return new PlotItem(m_curves, id);
  }
  else
  {
    obj = m_plotObjects->getPlotObject(id);
    if ( obj )
    {
      return new PlotItem(m_plotObjects, id);
    }
  }
  return nullptr;
}


/**
 * Copy a curve from another plot. Puts a PlotItem referencing the curve into 
 * this plot.
 */
void Plot::copyCurve(Plot* plot, PlotObject::id_t id)
{
  auto curve = plot->getCurve(id);
  auto newCurve = dynamic_cast<FunctionCurve*>(curve->clone());
  if ( newCurve ) addCurve( newCurve );
}

/**
 * Remove a curve.
 * @param id :: ID of a curve to remove.
 */
void Plot::removeCurve(PlotObject::id_t id)
{
  auto items = getAllCurveItems();
  foreach(PlotItem* item, items)
  {
    if ( item->getID() == id && item->getWorkspace() == m_curves )
    {
      m_curves->removeObject(id);
      item->detach();
      delete item;
    }
  }
}

/**
  * Remove all curves.
  */
void Plot::removeAllCurves()
{
    auto items = getAllCurveItems();
    foreach(PlotItem* item, items)
    {
      if ( item->getWorkspace() == m_curves )
      {
        m_curves->removeObject(item->getID());
        item->detach();
        delete item;
      }
    }
}

/**
 * Save to a file as an image
 * @param fileName :: Name of a file to save to.
 */
void Plot::saveAsImage(const QString& fileName)
{
  QPixmap image(this->size());
  image.fill();
  this->print(image);
  image.save(fileName);
}

/**
 * Save to a PDF file
 * @param fileName :: Name of a file to save to.
 */
void Plot::saveAsPDF(const QString& fileName)
{
  QPrinter printer;
  printer.setDocName ("Test");
  printer.setCreator("Test");
  printer.setFullPage(true);
  printer.setOutputFileName(fileName);
  printer.setOutputFormat(QPrinter::PdfFormat);
  print(printer);

}


} // QtAPI
