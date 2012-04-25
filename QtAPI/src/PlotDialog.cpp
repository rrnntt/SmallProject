#include "QtAPI/PlotDialog.h"
#include "ui_PlotDialog.h"
#include "QtAPI/Plot.h"
#include "QtAPI/PlotDefaults.h"
#include "QtAPI/FunctionCurve.h"

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
}

void PlotDialog::initCurvePage()
{
  auto curveIDs = m_plot->getCurveIDs();
  if ( curveIDs.isEmpty() ) return;
  foreach(PlotObject::id_t id, curveIDs)
  {
    if ( id == 0 ) continue;
    ui->cbCurve->addItem(QString::number(id),id);
  }

  ui->cbStyle->addItem("NoCurve",FunctionCurve::NoCurve);
  ui->cbStyle->addItem("Lines",FunctionCurve::Lines);
  ui->cbStyle->addItem("Sticks",FunctionCurve::Sticks);
  ui->cbStyle->addItem("Steps",FunctionCurve::Steps);
  ui->cbStyle->addItem("Dots",FunctionCurve::Dots);
  auto ci = ui->cbCurve->currentIndex();
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
  }
}

void PlotDialog::accept()
{
  apply();
  close();
}

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

} // QtAPI
