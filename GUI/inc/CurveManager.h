#ifndef GUI_CURVEMANAGER_H
#define GUI_CURVEMANAGER_H

#include "API/Singleton.h"
#include "qwt_plot_curve.h"

#include <QObject>
#include <boost/shared_ptr.hpp>
#include <map>

class CurveManager: public QObject, protected API::Singleton
{
  Q_OBJECT
public:
  ~CurveManager(){}
  virtual std::string name() const {return "CurveManager";}
  static CurveManager& instance();
protected:
  CurveManager();
  std::map<size_t,boost::shared_ptr<QwtPlotCurve> > m_map;

};

#endif // GUI_CURVEMANAGER_H
