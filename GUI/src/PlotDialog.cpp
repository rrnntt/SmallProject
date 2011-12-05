#include "PlotDialog.h"
#include "ui_PlotDialog.h"
#include "Plot.h"
#include "PlotDefaults.h"

// qwt includes
#include "qwt_scale_div.h"
#include "qwt_text.h"
#include "qwt_legend.h"

#include <iostream>
#include <stdexcept>

PlotDialog::PlotDialog(QWidget *parent, Plot* plot) :
    QDialog(parent),
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
}

void PlotDialog::accept()
{
  apply();
  close();
}

void PlotDialog::apply()
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

  m_plot->replot();
  m_plot->polish();
}
