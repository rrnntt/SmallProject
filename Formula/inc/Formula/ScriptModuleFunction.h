#ifndef FORMULA_SCRIPTMODULEFUNCTION_H
#define FORMULA_SCRIPTMODULEFUNCTION_H

#include "Formula/DllExport.h"
#include "Formula/Function.h"

namespace Formula
{

class ScriptFunction;

class FORMULA_EXPORT ScriptModuleFunction: public Function
{
public:
    ScriptModuleFunction(boost::shared_ptr<ScriptFunction> sf);
    std::string type()const{return "ScriptModuleFunction";}
    int testArguments(std::vector<Argument>& args);
    const Variable& eval()const;
    const Variable& operator()()const;
private:
    Variable_ptr m_return;
    boost::shared_ptr<ScriptFunction> m_scriptFunction;
    std::vector<std::string> m_argTypes;
};

} // Formula

#endif // FORMULA_SCRIPTMODULEFUNCTION_H
