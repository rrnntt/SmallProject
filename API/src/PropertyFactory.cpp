#include "API/PropertyFactory.h"
#include "Kernel/Framework.h"

namespace API
{

PropertyFactory::PropertyFactory()
{
}

PropertyFactory& PropertyFactory::instance()
{
  Singleton* s = Kernel::Framework::instance().getSingleton("PropertyFactory");
  if (s == nullptr)
  {
    PropertyFactory *f = new PropertyFactory();
    Kernel::Framework::instance().registerSingleton("PropertyFactory",f);
    return *f;
  }
  else
  {
    return *static_cast<PropertyFactory*>(s);
  }
}

} // API
