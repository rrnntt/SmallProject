#ifndef NAMESPACE_H
#define NAMESPACE_H

#include "Formula/DllExport.h"
#include "Formula/Variable.h"

#include <boost/shared_ptr.hpp>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <stdexcept>

namespace Formula
{

  class Function;
  class Expression;
  //class FunctionFactory;

class FORMULA_EXPORT Namespace
{
public:
    Namespace();
    Namespace(const Namespace& parent,const std::string& nm);///< not a copy constructor
    virtual ~Namespace();
    const std::string& name()const{return m_name;}
    void setName(const std::string& nm){m_name = nm;}
    boost::shared_ptr<Namespace> childNamespace(const std::string& nm){return m_namespaces[nm];}
    boost::shared_ptr<Namespace> addNamespace(Namespace* ns);
    void addNamespace(boost::shared_ptr<Namespace> nsptr);
    size_t namespaceCount()const{return m_namespaces.size();}
    typedef std::map<std::string, boost::shared_ptr<Namespace> >::iterator Namespace_it;
    Namespace_it namespaceBegin(){return m_namespaces.begin();}
    Namespace_it namespaceEnd(){return m_namespaces.end();}

    Variable_ptr addVariable(const std::string& type,const std::string& name,const std::string& value = "");
    Variable_ptr addVariable(Variable_ptr var,const std::string& name,const std::string& value = "");
    bool setVariable(const std::string& name,const std::string& value);
    bool setVariable(const std::string& name,const Variable& value);
    bool setVariable(const std::string& name,const double* value);
    /// Return pointer to a variable named name previously added with addVariable
    Variable_ptr getVariable(const std::string& name);
    /// Return pointer to a function named name
    boost::shared_ptr<Function> getFunction(const std::string& name)const;
    /// Add a local funtion
    void addFunction(const std::string& funName,Function* funNew);

    bool varExists(const std::string& nam)const;
    bool funExists(const std::string& nam)const;
    std::vector<Variable_ptr> getVars()const;
    void logVars()const;
    void logFuns()const;
    static bool isNumber(const std::string& nam);
    bool hasValue(const std::string& nam,bool checkVisible = true)const;
    //bool isVisible(const std::string& nam)const;
    // All visible var names
    std::set<std::string> getVarNames()const;
    // Visible var names with values
    std::set<std::string> getSetVarNames()const;
    // Visible var names with no values
    std::set<std::string> getFreeNames()const;
    // Visible var names with no values
    std::set<std::string> getFreeNames(boost::shared_ptr<const Expression> e1)const;
    bool areNumEquiv(boost::shared_ptr<const Expression> e1,
                     boost::shared_ptr<const Expression> e2,
                     double absTol = -1,
                     double relTol = -1);

    Variable_ptr True;

protected:

    /// Object holder containing a pointer to an object as well as some flags and information
    struct ObjectHolder
    {
      ObjectHolder():isConstant(false),hasValue(false){}
      boost::shared_ptr<Variable> obj;
      bool isConstant;
      bool hasValue;
    };
    std::string m_name;
    std::map<std::string, boost::shared_ptr<Namespace> > m_namespaces;
    std::vector< boost::shared_ptr<Namespace> > m_anonymous_namespaces;
    Namespace* m_parent;
    //boost::shared_ptr<FunctionFactory> m_funFactory;
    /* Variables */
    std::map<std::string,ObjectHolder> m_variables;
    typedef std::map<std::string,ObjectHolder>::iterator var_it;
    typedef std::map<std::string,ObjectHolder>::const_iterator var_cit;
    /* Functions */
    std::map<std::string,boost::shared_ptr<Function> > m_functions;
    typedef std::map<std::string,boost::shared_ptr<Function> >::iterator fun_it;
    typedef std::map<std::string,boost::shared_ptr<Function> >::const_iterator fun_cit;

};

typedef boost::shared_ptr<Namespace> Namespace_ptr;

}

#endif // NAMESPACE_H
