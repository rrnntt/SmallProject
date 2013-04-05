#include "Numeric/DomainFactory.h"
#include "Kernel/Framework.h"

namespace Numeric
{

/**
 * Constructor.
 */
DomainFactory::DomainFactory():Kernel::DynamicFactory<DomainCreator>()
{
}

/**
 * Return reference to the instance of the singleton. On first call the singleton 
 * is registered with API::Framework.
 */
DomainFactory& DomainFactory::instance()
{
  Kernel::Singleton* s = Kernel::Framework::instance().getSingleton("DomainFactory");
  if (s == nullptr)
  {
    DomainFactory *f = new DomainFactory();
    Kernel::Framework::instance().registerSingleton("DomainFactory",f);
    return *f;
  }
  else
  {
    return *static_cast<DomainFactory*>(s);
  }
}

} // Numeric

