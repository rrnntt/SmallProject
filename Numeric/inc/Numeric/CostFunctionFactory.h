#ifndef NUMERIC_COSTFUNCTIONFACTORY_H
#define NUMERIC_COSTFUNCTIONFACTORY_H

#include "Numeric/DllExport.h"
#include "Numeric/ICostFunction.h"
#include "Kernel/Singleton.h"
#include "Kernel/DynamicFactory.h"

namespace Numeric
{

/**
 * A factory for dynamically creating instances of ICostFunction.
 */
class NUMERIC_EXPORT CostFunctionFactory: public Kernel::DynamicFactory<ICostFunction>, protected Kernel::Singleton
{
public:
  virtual std::string name() const {return "CostFunctionFactory";}
  static CostFunctionFactory& instance();
protected:
  CostFunctionFactory();
};

} // NUMERIC

/// A macro to declare a new type of ICostFunction that can be created by this factory
#define DECLARE_COSTFUNCTION(type) \
int declare_icostfunction_##type( (Numeric::CostFunctionFactory::instance().subscribe<type>(#type),0));

#endif // NUMERIC_COSTFUNCTIONFACTORY_H
