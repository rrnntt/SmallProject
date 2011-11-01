#ifndef VARIABLEFACTORY_H
#define VARIABLEFACTORY_H

#include "Formula/DllExport.h"
//#include "SingletonHolder.h"
#include "Kernel/DynamicFactory.h"
#include "Formula/Variable.h"

namespace Formula
{

class FORMULA_EXPORT VariableFactory: public Kernel::DynamicFactory<Variable>
{
  VariableFactory(){}
public:
  static VariableFactory& instance();
};

#define DECLARE_VARIABLE(name) \
int declare_variable_##name( (VariableFactory::instance().subscribe<name>(#name),0));

} // Formula

#endif // VARIABLEFACTORY_H
