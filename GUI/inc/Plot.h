#ifndef PLOT_H
#define PLOT_H

#include "qwt_plot.h"
#include <boost/shared_ptr.hpp>

namespace Formula
{
  class Expression;
}

class Plot: public QwtPlot
{
public:
  Plot(QWidget *parent = NULL);
  //Plot(boost::shared_ptr<const Formula::Expression> expr,QWidget *parent = NULL);

  //void addExpression(boost::shared_ptr<const Formula::Expression> expr);
protected:
  void init();
private:
  int m_curve_count;
};

#endif // PLOT_H
