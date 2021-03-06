#include "QtAPI/CurveManager.h"

#include "Kernel/Framework.h"

namespace QtAPI
{

CurveManager::CurveManager()
{
}

CurveManager& CurveManager::instance()
{
  Singleton* s = Kernel::Framework::instance().getSingleton("CurveManager");
  if (s == nullptr)
  {
    CurveManager *f = new CurveManager();
    Kernel::Framework::instance().registerSingleton("CurveManager",f);
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
