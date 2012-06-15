#include "Numeric/FuncMinimizerFactory.h"
#include "Kernel/Framework.h"

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
  Kernel::Singleton* s = Kernel::Framework::instance().getSingleton("FuncMinimizerFactory");
  if (s == nullptr)
  {
    FuncMinimizerFactory *f = new FuncMinimizerFactory();
    Kernel::Framework::instance().registerSingleton("FuncMinimizerFactory",f);
    return *f;
  }
  else
  {
    return *static_cast<FuncMinimizerFactory*>(s);
  }
}

} // Numeric

