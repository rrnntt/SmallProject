#ifndef API_COLUMNFACTORY_H
#define API_COLUMNFACTORY_H

#include "API/DllExport.h"
#include "API/Column.h"
#include "Kernel/Singleton.h"
#include "Kernel/DynamicFactory.h"

namespace API
{

class API_EXPORT ColumnFactory: public Kernel::DynamicFactory<Column>, protected Kernel::Singleton
{
public:
  virtual std::string name() const {return "ColumnFactory";}
  static ColumnFactory& instance();
  Column* createColumn(const std::string& type);
};

} // DataObjects

#define DECLARE_COLUMN(DataType,TypeName) \
    namespace{ \
    int register_column_##TypeName(  \
    (API::ColumnFactory::instance().subscribe< DataType >(#TypeName),0)); \
    }

#endif // API_COLUMNFACTORY_H
