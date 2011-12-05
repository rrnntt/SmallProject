#include "CurveManager.h"
#include "API/Framework.h"

#include <iostream>

CurveManager::CurveManager(const std::string& name):Kernel::DataService<QwtPlotCurve>(name)
{
}

CurveManager& CurveManager::instance()
{
  API::Singleton* s = API::Framework::instance().getSingleton("CurveManager");
  if (s == nullptr)
  {
    CurveManager *f = new CurveManager("CurveManager");
    API::Framework::instance().registerSingleton("CurveManager",f);
    return *f;
  }
  else
  {
    return *static_cast<CurveManager*>(s);
  }
}
