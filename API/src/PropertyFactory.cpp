#include "API/PropertyFactory.h"
#include "API/Framework.h"

namespace API
{

PropertyFactory::PropertyFactory()
{
}

PropertyFactory& PropertyFactory::instance()
{
  Singleton* s = Framework::instance().getSingleton("PropertyFactory");
  if (s == nullptr)
  {
    PropertyFactory *f = new PropertyFactory();
    Framework::instance().registerSingleton("PropertyFactory",f);
    return *f;
  }
  else
  {
    return *static_cast<PropertyFactory*>(s);
  }
}

} // API
