#ifndef QTAPI_PLOT_H
#define QTAPI_PLOT_H

#include "DllExport.h"
#include "qwt_plot.h"
#include <boost/shared_ptr.hpp>

namespace QtAPI
{

class QTAPI_EXPORT Plot: public QwtPlot
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

} // QtAPI

#endif // QTAPI_PLOT_H
