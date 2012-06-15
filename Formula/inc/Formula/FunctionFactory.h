#ifndef FUNCTIONFACTORY_H
#define FUNCTIONFACTORY_H

#include "Formula/DllExport.h"
#include "Formula/Function.h"
#include "Kernel/DynamicFactory.h"
#include "Kernel/Singleton.h"

namespace Formula
{

class FORMULA_EXPORT FunctionFactory: public Kernel::DynamicFactory<Function>, protected Kernel::Singleton
{
public:
  /// constructor
  FunctionFactory();
  virtual std::string name() const {return "Formula::FunctionFactory";}
  /// Returns reference to the global instance of the factory
  static FunctionFactory& instance();
  template<class F>
  void subscribeFunction(const std::string& fun,const std::string& arg_types);
  Function* create(const std::string& fun,std::vector<Argument>& args);
private:
  typedef std::multimap<std::string,std::string> ArgMapType;
  typedef std::pair<ArgMapType::iterator,ArgMapType::iterator> ArgRange;
  ArgMapType m_arg_types;
};

/** Subscribe a function to the factory.
  * @param fun Function name
  * @param arg_types Types of the arguments. String names for Variable types separated by commas,
  *  in the right order. The last entry can be "..." meaning any number of arguments of the type
  *  of the previous entry. NO SPACES
  */
template<class F>
void FunctionFactory::subscribeFunction(const std::string& fun,const std::string& arg_types)
{
  ArgRange range = m_arg_types.equal_range(fun);
  for(ArgMapType::iterator it=range.first;it!=range.second;++it)
  {
    if (it->second == arg_types)
    {
      throw std::invalid_argument("Function "+fun+" with arguments "+arg_types+" already exists");
    }
  }
  m_arg_types.insert(std::make_pair(fun,arg_types));
  subscribe<F>(fun+","+arg_types);
}

#define DECLARE_FUNCTION(funClass,funName,argType) \
int declare_variable_##funClass( (FunctionFactory::instance().subscribeFunction<funClass>(funName,argType),0));

} // Formula

#endif // FUNCTIONFACTORY_H
