#include "Numeric/FuncMinimizerFactory.h"
#include "API/Framework.h"

namespace Numeric
{

/**
 * Constructor.
 */
//FuncMinimizerFactory::FuncMinimizerFactory()
//{
//}

/**
 * Return reference to the instance of the singleton. On first call the singleton 
 * is registered with API::Framework.
 */
FuncMinimizerFactory& FuncMinimizerFactory::instance()
{
  API::Singleton* s = API::Framework::instance().getSingleton("FuncMinimizerFactory");
  if (s == nullptr)
  {
    FuncMinimizerFactory *f = new FuncMinimizerFactory();
    API::Framework::instance().registerSingleton("FuncMinimizerFactory",f);
    return *f;
  }
  else
  {
    return *static_cast<FuncMinimizerFactory*>(s);
  }
}

} // Numeric

