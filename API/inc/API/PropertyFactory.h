#ifndef API_PROPERTYFACTORY_H
#define API_PROPERTYFACTORY_H

#include "Kernel/DynamicFactory.h"
#include "Kernel/Property.h"
#include "API/DllExport.h"
#include "API/Workspace.h"
#include "Kernel/Singleton.h"

namespace API
{

class API_EXPORT PropertyFactory: public Kernel::DynamicFactory<Kernel::Property>, protected Kernel::Singleton
{
  PropertyFactory();
public:
  virtual std::string name() const {return "PropertyFactory";}
  static PropertyFactory& instance();
};

} // API

#define DECLARE_PROPERTY(type,type_name) \
int declare_property_##type( (API::PropertyFactory::instance().subscribe<type>(#type_name),0));

#endif // API_WORKSPACEFACTORY_H
