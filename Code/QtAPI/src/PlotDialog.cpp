#include "QtAPI/PlotDialog.h"
#include "ui_PlotDialog.h"
#include "QtAPI/Plot.h"
#include "QtAPI/PlotDefaults.h"
#include "QtAPI/FunctionCurve.h"
#include "QtAPI/WindowManager.h"

// qwt includes
#include "qwt_scale_div.h"
#include "qwt_text.h"
#include "qwt_legend.h"

#include <iostream>
#include <stdexcept>

namespace QtAPI
{

PlotDialog::PlotDialog(Plot* plot) :
QDialog(plot),
ui(new Ui::PlotDialog),
m_plot(plot)
{
    if (!plot)
    {
      throw std::invalid_argument("Zero plot pointer");
    }

    ui->setupUi(this);

    connect(ui->btnApply,SIGNAL(clicked()),this,SLOT(accept()));
    connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(reject()));

    init();
}

PlotDialog::~PlotDialog()
{
    delete ui;
}

void PlotDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void PlotDialog::init()
{
  ui->leCaption->setText(m_plot->title().text());
  QFont font = m_plot->title().font();
  ui->fcbTitleFont->setCurrentFont(font);
  ui->sbTitleFontSize->setValue(font.pixelSize());

  QwtScaleDiv* xscale = m_plot->axisScaleDiv(QwtPlot::xBottom);
  ui->leXTitle->setText(m_plot->axisTitle(QwtPlot::xBottom).text());
  ui->leXMin->setText(QString::number(xscale->lowerBound()));
  ui->leXMax->setText(QString::number(xscale->upperBound()));

  QwtScaleDiv* yscale = m_plot->axisScaleDiv(QwtPlot::yLeft);
  ui->leYTitle->setText(m_plot->axisTitle(QwtPlot::yLeft).text());
  ui->leYMin->setText(QString::number(yscale->lowerBound()));
  ui->leYMax->setText(QString::number(yscale->upperBound()));

  ui->cbShowLegend->setChecked(m_plot->legend() != NULL);
  
  initCurvePage();
  initCurveCopyPage();
}

void PlotDialog::initCurvePage()
{
  updateThisCurveList();

  ui->cbStyle->addItem("NoCurve",FunctionCurve::NoCurve);
  ui->cbStyle->addItem("Lines",FunctionCurve::Lines);
  ui->cbStyle->addItem("Sticks",FunctionCurve::Sticks);
  ui->cbStyle->addItem("Steps",FunctionCurve::Steps);
  ui->cbStyle->addItem("Dots",FunctionCurve::Dots);
  connect(ui->cbLineColor,SIGNAL(currentIndexChanged(int)),ui->cbSymbolColor,SLOT(setCurrentIndex(int)));
  connect(ui->cbCurve,SIGNAL(currentIndexChanged(int)),this,SLOT(updateCurve(int)));

  updateCurve( ui->cbCurve->currentIndex() );
}

/**
 * Update the list of curves in this plot. (ui->cbCurve)
 */
void PlotDialog::updateThisCurveList()
{
  ui->cbCurve->clear();
  auto curveIDs = m_plot->getCurveIDs();
  if ( curveIDs.isEmpty() ) return;
  foreach(PlotObject::id_t id, curveIDs)
  {
    if ( id == 0 ) continue;
    auto curve = m_plot->getCurve(id);
    if ( !curve ) continue;
    QString title = curve->getTitle();
    if ( title.isEmpty() )
    {
      title = QString::number(id);
    }
    ui->cbCurve->addItem(title,(qulonglong)id);
  }
}

/**
 * Init the page for copying curves from one plot to another
 */
void PlotDialog::initCurveCopyPage()
{
  auto subWindows = WindowManager::instance().getSubWindows();
  foreach(SubWindow* sw, subWindows)
  {
    QWidget* w = sw->getWidget();
    Plot* plot = dynamic_cast<Plot*>(w);
    if ( plot && plot != m_plot )
    {
      ui->cbPlots->addItem(plot->title().text(),(qulonglong)sw->getID());
    }
  }
  connect(ui->cbPlots,SIGNAL(currentIndexChanged(int)),this,SLOT(updateCurveList(int)));
  connect(ui->btnCopy,SIGNAL(clicked()),this,SLOT(copyCurve()));
  connect(ui->btnDelete,SIGNAL(clicked()),this,SLOT(deleteCurve()));
  updateCurveList(0);
  fillCurves(ui->lstThisCurves,m_plot);
}

/**
 * Apply the changes and close.
 */
void PlotDialog::accept()
{
  apply();
  close();
}

/**
 * Apply the changes on the current page 
 */
void PlotDialog::apply()
{
  switch(ui->tabWidget->currentIndex())
  {
  case 0:
    applyPlot();
    break;
  case 1:
    applyCurve();
    break;
  };
  m_plot->replot();
  m_plot->polish();
}

void PlotDialog::applyCurve()
{
  auto ci = ui->cbCurve->currentIndex();
  auto curve = m_plot->getCurve(ui->cbCurve->itemData(ci).toInt());
  if ( !curve ) return;
  auto style = ui->cbStyle->itemData(ui->cbStyle->currentIndex()).toInt();
  curve->setCurveStyle(static_cast<FunctionCurve::CurveStyle>(style));
  QPen pen = curve->pen();
  pen.setColor(ui->cbLineColor->color());
  pen.setStyle(ui->cbPenStyle->style());
  pen.setWidth(ui->sbLineWidth->value());
  curve->setPen(pen);
  QSize symbolSize(ui->sbSymbolSize->value(),ui->sbSymbolSize->value());
  QwtSymbol symbol(
    ui->cbSymbolShape->selectedSymbol(),
    QBrush(ui->cbSymbolColor->color()),
    QPen(pen.color()),
    symbolSize);
  curve->setSymbol(symbol);
}

void PlotDialog::applyPlot()
{
  // Set new title
  QwtText Title(ui->leCaption->text());
  QFont font(ui->fcbTitleFont->currentFont());
  font.setPixelSize(ui->sbTitleFontSize->value());
  Title.setFont(font);
  m_plot->setTitle(Title);
  if (ui->cbSetDefaultTitleFont->isChecked())
  {
    PlotDefaults::instance().setTitleFont(font);
  }

  m_plot->setAxisScale(QwtPlot::xBottom,ui->leXMin->text().toDouble(),ui->leXMax->text().toDouble());
  m_plot->setAxisTitle(QwtPlot::xBottom,ui->leXTitle->text());
  m_plot->setAxisScale(QwtPlot::yLeft,ui->leYMin->text().toDouble(),ui->leYMax->text().toDouble());
  m_plot->setAxisTitle(QwtPlot::yLeft,ui->leYTitle->text());

  bool hasLegend = m_plot->legend() != NULL;
  if (ui->cbShowLegend->isChecked())
  {
    if (!hasLegend)
    {
      m_plot->insertLegend(new QwtLegend(),QwtPlot::RightLegend);
    }
  }
  else
  {
    if (hasLegend)
    {
      m_plot->insertLegend(NULL,QwtPlot::RightLegend);
    }
  }

}

/**
 * Fills a list widget with curves from a plot. User data contain curve id;
 * @param lst :: List widget to display the curves
 * @param plot :: Plot to take the curves from
 */
void PlotDialog::fillCurves(QListWidget* lst, Plot* plot)
{
  lst->clear();
  auto curves = plot->getCurveIDs();
  for(auto c = curves.begin(); c != curves.end(); ++c)
  {
    auto curve = plot->getCurve(*c);
    if ( !curve ) continue;
    QString title = curve->getTitle();
    if ( title.isEmpty() )
    {
      title = QString::number(*c);
    }
    auto item = new QListWidgetItem(title,lst);
    item->setData(Qt::UserRole,static_cast<int>(*c));
  }
}

/**
 * Refill the other plot's curves combo.
 * @param i :: Index of the plot in plots combo box
 */
void PlotDialog::updateCurveList(int i)
{
  ui->lstOtherCurves->clear();
  size_t id = ui->cbPlots->itemData(i).toInt();
  auto sw = WindowManager::instance().getSubWindow(id);
  if ( sw )
  {
    Plot* plot = dynamic_cast<Plot*>(sw->getWidget());
    if ( !plot ) return;
    fillCurves(ui->lstOtherCurves, plot);
  }
}

/**
 * Copy a curve from another plot to this one.
 */
void PlotDialog::copyCurve()
{
  auto item = ui->lstOtherCurves->currentItem();
  if ( !item ) return;
  size_t id = ui->cbPlots->itemData(ui->cbPlots->currentIndex()).toInt();
  auto sw = WindowManager::instance().getSubWindow(id);
  if ( !sw ) return;
  Plot* plot = dynamic_cast<Plot*>(sw->getWidget());
  if ( !plot ) return;
  int ci = item->data(Qt::UserRole).toInt();
  m_plot->copyCurve(plot,ci);
  m_plot->delayedReplot();
  fillCurves(ui->lstThisCurves,m_plot);
  updateThisCurveList();
}

/**
 * Delete a curve from this plot.
 */
void PlotDialog::deleteCurve()
{
  auto item = ui->lstThisCurves->currentItem();
  if ( !item ) return;
  int id = item->data(Qt::UserRole).toInt();
  m_plot->removeCurve(id);
  m_plot->delayedReplot();
  fillCurves(ui->lstThisCurves,m_plot);
  updateThisCurveList();
}

/**
 * Update curve data on Curve page.
 */
void PlotDialog::updateCurve(int ci)
{
  auto curve = m_plot->getCurve(ui->cbCurve->itemData(ci).toInt());
  if (curve)
  {
    int si = ui->cbStyle->findData(curve->getCurveStyle());
    if ( si >= 0 )
    {
      ui->cbStyle->setCurrentIndex(si);
    }
    QPen pen = curve->pen();
    ui->cbLineColor->setColor(pen.color());
    ui->cbPenStyle->setStyle(pen.style());
    ui->sbLineWidth->setValue(pen.width());

    auto symbol = curve->symbol();
    ui->cbSymbolShape->setSymbolStyle(symbol.style());
    ui->cbSymbolColor->setColor(symbol.brush().color());
    ui->sbSymbolSize->setValue(symbol.size().width());
  }
}

} // QtAPI
