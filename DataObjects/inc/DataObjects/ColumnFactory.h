#ifndef DATAOBJECTS_COLUMNFACTORY_H
#define DATAOBJECTS_COLUMNFACTORY_H

#include "DataObjects/DllExport.h"
#include "DataObjects/Column.h"
#include "API/Singleton.h"
#include "Kernel/DynamicFactory.h"

namespace DataObjects
{

class DATAOBJECTS_EXPORT ColumnFactory: public Kernel::DynamicFactory<Column>, protected API::Singleton
{
public:
  virtual std::string name() const {return "ColumnFactory";}
  static ColumnFactory& instance();
};

} // DataObjects

#define DECLARE_COLUMN(DataType,TypeName) \
    namespace{ \
    int register_column_##TypeName(  \
    (DataHandling::ColumnFactory::instance().subscribe< DataType >(#TypeName),0)); \
    }

#endif // DATAOBJECTS_COLUMNFACTORY_H
