#include "Plot.h"
#include "PlotDefaults.h"
//#include "SimpleFormula/Expression.h"
//#include "ExpressionCurve.h"

#include "qwt_plot_canvas.h"
#include "qwt_plot_layout.h"
#include "qwt_scale_draw.h"
#include "qwt_text.h"
#include "qwt_legend.h"

#include <QPalette>
#include <iostream>

using namespace Formula;

Plot::Plot(QWidget *parent)
:QwtPlot(parent),
m_curve_count()
{
  init();
}

//Plot::Plot(boost::shared_ptr<const Expression> expr,QWidget *parent)
//{
//  int i = 0;
//  bool isList = expr->isFunction() && (expr->name() == "List" || expr->name().empty() || expr->name() == ",");
//  Expression_cptr e = isList ? expr->terms()[0] : expr;
//  bool axesSet = false;
//  do
//  {
//    if (i > 0)
//    {
//      e = expr->terms()[i];
//    }
//    if (!e->isEquation()) continue;
//    if (!(*e)[0].isVariable()) continue;
//    if (!axesSet)
//    {
//      setAxisTitle(yLeft,QString::fromStdString((*e)[0].name()));
//      std::set<std::string> freeVars = (*e)[1].getFreeVarNames();
//      if (freeVars.size() != 1) continue;
//      setAxisTitle(xBottom,QString::fromStdString(*freeVars.begin()));
//      axesSet = true;
//    }
//    ExpressionCurve* ec = new ExpressionCurve(e);
//    ec->attach(this);
//  }
//  while(isList && ++i < expr->size());
//  init();
//}

void Plot::init()
{
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

  //insertLegend(new QwtLegend(),QwtPlot::RightLegend); // to create legend
  //insertLegend(NULL,QwtPlot::RightLegend); // to delete legend
}

/**
  * Adds a curve defined by an expression. Expression must retrun a Scalar (or in future Vector for parametric curves)
  * and have 1 and only 1 free variable.
  * @param expr Expression to plot
  */
//void Plot::addExpression(boost::shared_ptr<const Formula::Expression> expr)
//{
//  ExpressionCurve* ec = new ExpressionCurve(expr);
//  ec->attach(this);
//  ec->setPen(QPen(PlotDefaults::instance().getCurveColor(m_curve_count++)));
//  ec->setTitle(QString::fromStdString(expr->str()));
//
//}
