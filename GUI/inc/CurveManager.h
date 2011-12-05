#ifndef CURVEMANAGER_H
#define CURVEMANAGER_H

#include "qwt_plot_curve.h"
#include "Kernel/DataService.h"
#include "API/Singleton.h"

class CurveManager: public Kernel::DataService<QwtPlotCurve>, protected API::Singleton
{
public:
  virtual std::string name() const {return "CurveManager";}
  static CurveManager& instance();
protected:
  CurveManager(const std::string& name);

};

#endif /*CURVEMANAGER_H*/