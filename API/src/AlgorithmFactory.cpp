#include "API/AlgorithmFactory.h"
#include "API/Framework.h"

namespace API
{

AlgorithmFactory::AlgorithmFactory()
{
}

AlgorithmFactory& AlgorithmFactory::instance()
{
  Singleton* s = Framework::instance().getSingleton("AlgorithmFactory");
  if (s == nullptr)
  {
    AlgorithmFactory *f = new AlgorithmFactory();
    Framework::instance().registerSingleton("AlgorithmFactory",f);
    return *f;
  }
  else
  {
    return *static_cast<AlgorithmFactory*>(s);
  }
}

} // API
