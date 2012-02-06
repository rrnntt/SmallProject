#ifndef QTAPI_CURVEMANAGER_H
#define QTAPI_CURVEMANAGER_H

#include "QtAPI/DllExport.h"
#include "API/Singleton.h"
#include "QtAPI/PlotCurve.h"

#include <QObject>
#include <QPointer>
#include <QList>

namespace QtAPI
{

class QTAPI_EXPORT CurveManager: public QObject, protected API::Singleton
{
  Q_OBJECT
public:
  ~CurveManager(){}
  virtual std::string name() const {return "CurveManager";}
  static CurveManager& instance();
  void addCurve(PlotCurve* curve);
protected:
  CurveManager();
  QList< QPointer<PlotCurve> > m_map;

};

} // QtAPI

#endif // QTAPI_CURVEMANAGER_H
