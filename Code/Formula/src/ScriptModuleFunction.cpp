#include "Formula/ScriptModuleFunction.h"
#include "Formula/ScriptLine.h"
#include "Formula/Namespace.h"

#include <iostream>

namespace Formula
{

ScriptModuleFunction::ScriptModuleFunction(boost::shared_ptr<ScriptFunction> sf):
m_scriptFunction(sf)
{
  try
  {
    m_return = sf->getLocalNamespace_ptr()->getVariable("return");
  }
  catch(...){}
  const std::vector<std::string>& argNames = sf->arguments();
  for(int i = 0; i < argNames.size(); ++i)
  {
    std::string argType = sf->getLocalNamespace_ptr()->getVariable(argNames[i])->type();
    m_argTypes.push_back(argType);
  }

}

int ScriptModuleFunction::testArguments(std::vector<Argument>& args)
{
  if (args.size() != m_argTypes.size())
  {
    return 1;
  }
  for(size_t i=0;i<args.size();++i)
  {
    if (args[i]().type() != m_argTypes[i]) return i+1;
  }
  return 0;
}

const Variable& ScriptModuleFunction::eval()const
{
  const std::vector<std::string>& argNames = m_scriptFunction->arguments();
  if (m_argv.size() != argNames.size())
  {
    throw std::invalid_argument("Wrong number of arguments");
  }
  for(int i = 0; i < argNames.size(); ++i)
  {
    m_scriptFunction->setArgument(argNames[i],m_argv[i]());
  }
  m_scriptFunction->eval();
  return (*this)();
}

const Variable& ScriptModuleFunction::operator()()const
{
  return m_return ? *m_return : *m_scriptFunction->getLocalNamespace().True;
}

} // Formula
