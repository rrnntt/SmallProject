#include "CurveManager.h"

#include "API/Framework.h"

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

