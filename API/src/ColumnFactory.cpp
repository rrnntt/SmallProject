#include "API/ColumnFactory.h"
#include "API/Framework.h"

namespace API
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

  Column* ColumnFactory::createColumn(const std::string& type)
  {
    Column* column = create(type);
    column->m_type = type;
    return column;
  }

} // API
