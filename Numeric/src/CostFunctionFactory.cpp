#include "Numeric/CostFunctionFactory.h"
#include "Kernel/Framework.h"

namespace Numeric
{

/**
 * Constructor.
 */
CostFunctionFactory::CostFunctionFactory():Kernel::DynamicFactory<ICostFunction>()
{
}

/**
 * Return reference to the instance of the singleton. On first call the singleton 
 * is registered with API::Framework.
 */
CostFunctionFactory& CostFunctionFactory::instance()
{
  Kernel::Singleton* s = Kernel::Framework::instance().getSingleton("CostFunctionFactory");
  if (s == nullptr)
  {
    CostFunctionFactory *f = new CostFunctionFactory();
    Kernel::Framework::instance().registerSingleton("CostFunctionFactory",f);
    return *f;
  }
  else
  {
    return *static_cast<CostFunctionFactory*>(s);
  }
}

} // Numeric

