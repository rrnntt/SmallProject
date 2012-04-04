#include "Numeric/CostFunctionFactory.h"
#include "API/Framework.h"

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
  API::Singleton* s = API::Framework::instance().getSingleton("CostFunctionFactory");
  if (s == nullptr)
  {
    CostFunctionFactory *f = new CostFunctionFactory();
    API::Framework::instance().registerSingleton("CostFunctionFactory",f);
    return *f;
  }
  else
  {
    return *static_cast<CostFunctionFactory*>(s);
  }
}

} // Numeric

