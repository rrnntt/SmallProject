#include "DataObjects/ColumnFactory.h"
#include "API/Framework.h"

namespace DataObjects
{

  ColumnFactory& ColumnFactory::instance()
  {
    API::Singleton* s = API::Framework::instance().getSingleton("ColumnFactory");
    if (s == nullptr)
    {
      ColumnFactory *f = new ColumnFactory();
      API::Framework::instance().registerSingleton("ColumnFactory",f);
      return *f;
    }
    else
    {
      return *static_cast<ColumnFactory*>(s);
    }
  }

} // DataObjects
