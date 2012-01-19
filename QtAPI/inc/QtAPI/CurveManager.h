#ifndef QTAPI_CURVEMANAGER_H
#define QTAPI_CURVEMANAGER_H

#include "QtAPI/DllExport.h"
#include "API/Singleton.h"
#include "qwt_plot_curve.h"

#include <QObject>
#include <boost/shared_ptr.hpp>
#include <vector>

namespace QtAPI
{

class QTAPI_EXPORT CurveManager: public QObject, protected API::Singleton
{
  Q_OBJECT
public:
  ~CurveManager(){}
  virtual std::string name() const {return "CurveManager";}
  static CurveManager& instance();
protected:
  CurveManager();
  std::vector< boost::shared_ptr<QwtPlotCurve> > m_map;

};

} // QtAPI

#endif // QTAPI_CURVEMANAGER_H
