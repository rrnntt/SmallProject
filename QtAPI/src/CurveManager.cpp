#include "QtAPI/CurveManager.h"

#include "API/Framework.h"

namespace QtAPI
{

CurveManager::CurveManager()
{
}

CurveManager& CurveManager::instance()
{
  Singleton* s = API::Framework::instance().getSingleton("CurveManager");
  if (s == nullptr)
  {
    CurveManager *f = new CurveManager();
    API::Framework::instance().registerSingleton("CurveManager",f);
    return *f;
  }
  else
  {
    return *static_cast<CurveManager*>(s);
  }
}

void CurveManager::addCurve(PlotCurve* curve)
{
  m_map.push_back(curve);
}

} // QtAPI
