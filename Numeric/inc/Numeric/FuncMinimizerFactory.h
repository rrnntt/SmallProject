#ifndef NUMERIC_FUNCMINIMIZERFACTORY_H
#define NUMERIC_FUNCMINIMIZERFACTORY_H

#include "Numeric/DllExport.h"
#include "Numeric/IFuncMinimizer.h"
#include "API/Singleton.h"
#include "Kernel/DynamicFactory.h"

namespace Numeric
{

/**
 * A factory for dynamically creating instances of IFuncMinimizer.
 */
class NUMERIC_EXPORT FuncMinimizerFactory: public Kernel::DynamicFactory<IFuncMinimizer>, protected API::Singleton
{
public:
  virtual std::string name() const {return "FuncMinimizerFactory";}
  static FuncMinimizerFactory& instance();
protected:
  //FuncMinimizerFactory();
};

} // NUMERIC

/// A macro to declare a new type of IFuncMinimizer that can be created by this factory
#define DECLARE_FUNCMINIMIZER(type) \
int declare_ifuncminimizer_##type( (Numeric::FuncMinimizerFactory::instance().subscribe<type>(#type),0));

#endif // NUMERIC_FUNCMINIMIZERFACTORY_H
