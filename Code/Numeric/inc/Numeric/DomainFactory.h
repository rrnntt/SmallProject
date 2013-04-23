#ifndef NUMERIC_DOMAINFACTORY_H
#define NUMERIC_DOMAINFACTORY_H

#include "Numeric/DllExport.h"
#include "Numeric/DomainCreator.h"
#include "Kernel/Singleton.h"
#include "Kernel/DynamicFactory.h"

namespace Numeric
{

/**
 * A factory for dynamically creating instances of DomainCreator.
 */
class NUMERIC_EXPORT DomainFactory: public Kernel::DynamicFactory<DomainCreator>, protected Kernel::Singleton
{
public:
  virtual std::string name() const {return "DomainFactory";}
  static DomainFactory& instance();
protected:
  DomainFactory();
};

} // NUMERIC

/// A macro to declare a new type of DomainCreator that can be created by this factory
#define DECLARE_DOMAINCREATOR(type) \
int declare_domaincreator_##type( (Numeric::DomainFactory::instance().subscribe<type>(#type),0));

#endif // NUMERIC_DOMAINFACTORY_H
