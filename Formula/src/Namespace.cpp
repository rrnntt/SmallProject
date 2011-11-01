#include "Formula/Namespace.h"
#include "Formula/Object.h"
#include "Formula/Str.h"

#include "Formula/VariableFactory.h"
#include "Formula/Function.h"
#include "Formula/Reference.h"

#include <iostream>
#include <locale>
#include <cmath>

namespace Formula
{

/// Destructor
Namespace::~Namespace()
{
}


/* ----------- Non-static members ------------*/

Namespace::Namespace()
:m_name(""),
 m_parent(NULL)
{
}

Namespace::Namespace(const Namespace& parent,const std::string& nm)
:m_name(nm),
m_parent(NULL)
{
  m_functions.insert(parent.m_functions.begin(),parent.m_functions.end());
}

boost::shared_ptr<Namespace> Namespace::addNamespace(Namespace* ns)
{
  Namespace_ptr nsptr(ns);
  addNamespace(nsptr);
  return nsptr;
}

void Namespace::addNamespace(Namespace_ptr nsptr)
{
  nsptr->m_parent = this;
  if (nsptr->name().empty())
  {
    m_anonymous_namespaces.push_back(nsptr);
  }
  else
  {
    if (m_namespaces.find(nsptr->name()) != m_namespaces.end())
    {
      throw std::runtime_error("Namespace "+nsptr->name()+" is already defined in "+name());
    }
    m_namespaces[nsptr->name()] = nsptr;
  }
}

/** 
  * Add a variable to the namespace.
  * @param type Type of the object e.g. Scalar or Vector, etc.
  * @param name Object's name
  */
Variable_ptr Namespace::addVariable(const std::string& type,const std::string& name,const std::string& value)
{

    var_it it = m_variables.find(name);
    if (it != m_variables.end())
    {
        std::cerr<<"Variable "<<name<<" already defined\n";
        if (!value.empty())
        {
          setVariable(name,value);
        }
        return it->second.obj;
    }
    Variable* v = NULL;
    if (type == "Ref")
    {
      Reference* ref(NULL);
      if (value.empty())
      {
        ref = new Reference();
      }
      else
      {
        Variable_ptr var = getVariable(value);
        if (!var)
        {
          throw std::runtime_error("Cannot set reference to "+value);
        }
        ref = new Reference(var);
      }
      v = ref;
    }
    else
    {
      v = VariableFactory::instance().create(type);
    }

    if (v)
    {
      ObjectHolder& var = m_variables[name];
      var.obj.reset(v);
      v->setName(name);
      if (!value.empty())
      {
        setVariable(name,value);
      }
      return var.obj;
    }
    return Variable_ptr();
}

/**
  * Add a variable created elsewhere to the namespace. This method is to be used for
  * defining variables which cannot use the default creator, ie variables-references.
  * Variables defined with this method always have values.
  * @param v A shared pointer to the variable.
  * @param name Variable's name
  * @param value Value for the created variable if not empty
  */
Variable_ptr Namespace::addVariable(Variable_ptr v,const std::string& name,const std::string& value)
{
  var_it it = m_variables.find(name);
  if (it != m_variables.end())
  {
    throw std::runtime_error("Cannot add variable "+name);
  }
  if (v)
  {
    ObjectHolder& var = m_variables[name];
    var.obj = v;
    v->setName(name);
    if (!value.empty())
    {
      setVariable(name,value);
    }
    return var.obj;
  }
  return Variable_ptr();
}

/** 
  * Set a new value to an existing variable.
  * @param name Variable's name
  * @param value The new value as a string
  */
bool Namespace::setVariable(const std::string& name,const std::string& value)
{
  if (name.find('.') != std::string::npos)
  {
    std::vector<std::string> class_member = Str::split(name,'.');
    if (class_member.empty()) return false;
    Namespace_ptr child_ns = childNamespace(class_member[0]);
    if (!child_ns) return false;
    return child_ns->setVariable(class_member[1],value);
  }
  var_it it = m_variables.find(name);
  if (it != m_variables.end())
  {
    Reference* ref = dynamic_cast<Reference*>(it->second.obj.get());
    if (ref)
    {
      if (varExists(value))
      {
        ref->reset(getVariable(value));
      }
      else
      {
        ref = NULL;
      }
    }
    if (!ref)
    {
      it->second.obj->setValue(value);
      it->second.hasValue = true;
      if (name == value)
      {
        it->second.isConstant = true;
      }
    }
    return true;
  }
  if (m_parent) return m_parent->setVariable(name,value);
  return false;
}

bool Namespace::setVariable(const std::string& name,const double* value)
{
  if (name.find('.') != std::string::npos)
  {
    std::vector<std::string> class_member = Str::split(name,'.');
    if (class_member.empty()) return false;
    Namespace_ptr child_ns = childNamespace(class_member[0]);
    if (!child_ns) return false;
    return child_ns->setVariable(class_member[1],value);
  }
    var_it it = m_variables.find(name);
    if (it != m_variables.end())
    {
      it->second.obj->setValue(value);
      it->second.hasValue = true;
      return true;
    }
    if (m_parent) return m_parent->setVariable(name,value);
    return false;
}

bool Namespace::setVariable(const std::string& name,const Variable& value)
{
  if (name.find('.') != std::string::npos)
  {
    std::vector<std::string> class_member = Str::split(name,'.');
    if (class_member.empty()) return false;
    Namespace_ptr child_ns = childNamespace(class_member[0]);
    if (!child_ns) return false;
    return child_ns->setVariable(class_member[1],value);
  }
    var_it it = m_variables.find(name);
    if (it != m_variables.end())
    {
      it->second.obj->setValue(value);
      it->second.hasValue = true;
      return true;
    }
    if (m_parent) return m_parent->setVariable(name,value);
    return false;
}

boost::shared_ptr<Variable> Namespace::getVariable(const std::string& name)
{
  if (name.find(':') != std::string::npos)
  {
    std::vector<std::string> class_member = Str::split(name,'.');
    if (class_member.empty()) return Variable_ptr();
    Namespace_ptr child_ns = childNamespace(class_member[0]);
    if (!child_ns) return Variable_ptr();
    return child_ns->getVariable(class_member[1]);
  }
    var_cit it = m_variables.find(name);
    if (it != m_variables.end())
    {
      return it->second.obj;
    }
    if (m_parent) return m_parent->getVariable(name);
    return Variable_ptr();
}

boost::shared_ptr<Function> Namespace::getFunction(const std::string& name)const
{
    fun_cit it = m_functions.find(name);
    if (it != m_functions.end())
    {
        return it->second;
    }
    //Function_ptr unknown(new UnknownFunction(this,name));
    //return unknown;
    return boost::shared_ptr<Function>();
}

void Namespace::logVars()const
{
  std::cerr<<"Namespace variables:\n";
  for(var_cit it = m_variables.begin();it != m_variables.end();++it)
  {
    if (!it->second.isConstant)
    {
      std::cerr<<it->second.obj->name()<<' ' << *(it->second.obj) <<std::endl;
    }
  }
  std::cerr<<std::endl;
}

void Namespace::logFuns()const
{
  std::cerr<<"Namespace functions:\n";
//  for(fun_cit it = m_functions.begin();it != m_functions.end();++it)
//  {
//    std::cerr<<it->second->name()<<std::endl;
//  }
}

bool Namespace::varExists(const std::string& nam)const
{
  return m_variables.find(nam) != m_variables.end();
}

bool Namespace::funExists(const std::string& nam)const
{
  return m_functions.find(nam) != m_functions.end();
}

/**
  * Return a list of visible variables
  */
std::vector<Variable_ptr> Namespace::getVars()const
{
  std::vector<Variable_ptr> res;
  for(var_cit it = m_variables.begin(); it != m_variables.end(); ++it)
  {
    res.push_back(it->second.obj);
  }
  return res;
}

/**
  * Tests if a string is a floating point or int number
  */
bool Namespace::isNumber(const std::string& nam)
{
  std::string num = nam;
  Str::trim(num);
  if (num.empty()) return false;
  if (!isdigit(num[0])) return false;
  int dotCount = 0;
  for(std::string::const_iterator it=num.begin();it!=num.end();++it)
  {
    if (*it == '.')
    {
      ++dotCount;
      if (dotCount > 1) return false;
    }
    else if (!isdigit(*it)) return false;
  }
  return true;
}

bool Namespace::hasValue(const std::string& nam,bool checkVisible)const
{
  var_cit it = m_variables.find(nam);
  if (it == m_variables.end()) return false;
  return it->second.hasValue;
}

// All visible var names
std::set<std::string> Namespace::getVarNames()const
{
  std::set<std::string> res;
  for(var_cit it = m_variables.begin(); it != m_variables.end(); ++it)
  {
    if (!it->second.isConstant)
    {
      res.insert(it->second.obj->name());
    }
  }
  return res;
}

// Visible var names with values
std::set<std::string> Namespace::getSetVarNames()const
{
  std::set<std::string> res;
  for(var_cit it = m_variables.begin(); it != m_variables.end(); ++it)
  {
    if (!it->second.isConstant && it->second.hasValue)
    {
      res.insert(it->second.obj->name());
    }
  }
  return res;
}

// Visible var names with no values
std::set<std::string> Namespace::getFreeNames()const
{
  std::set<std::string> res;
  for(var_cit it = m_variables.begin(); it != m_variables.end(); ++it)
  {
    if (!it->second.isConstant && !it->second.hasValue)
    {
      res.insert(it->second.obj->name());
    }
  }
  return res;
}

std::set<std::string> Namespace::getFreeNames(boost::shared_ptr<const Expression> e1)const
{
  std::set<std::string> res;
//  std::set<std::string> localNames = e1->getVarNames();
//  for(var_cit it = m_variables.begin(); it != m_variables.end(); ++it)
//  {
//    if (it->second.isVisible )
//    {
//      std::string nam = it->second.obj->name();
//      std::set<std::string>::iterator lit = localNames.find(nam);
//      if (lit != localNames.end())
//      {
//        if ( !it->second.hasValue )
//        {
//          res.insert(nam);
//        }
//        localNames.erase(lit);
//      }
//    }
//  }
//  if (!localNames.empty())
//  {
//    throw std::runtime_error("Undefined names in expression");
//  }
  return res;
}

/**
  * Check if two expressions are numerically equivalent
  * Results of their eval() methods are compared
  */
bool Namespace::areNumEquiv(boost::shared_ptr<const Expression> e1,
                 boost::shared_ptr<const Expression> e2,
                 double absTol,
                 double relTol)
{
  std::set<std::string> freeNames(getFreeNames(e1));
  std::set<std::string> names2 = getFreeNames(e2);
  freeNames.insert(names2.begin(),names2.end());
  int count = 0;
  for(;count < 10;)
  {
    try
    {
      std::set<std::string>::const_iterator it = freeNames.begin();
      for(;it!=freeNames.end();++it)
      {
        getVariable(*it)->setRandomValue();
      }
//      bool ok = e1->eval()->compare(*e2->eval(),absTol,relTol);
//      if (!ok) return false;
      ++count;
    }
    catch(...)
    {
    }
  }
  return true;
}

void Namespace::addFunction(const std::string& funName,Function* funNew)
{
  fun_it it = m_functions.find(funName);
  if (it != m_functions.end())
  {
    throw std::runtime_error("Function "+funName+" already exists");
  }
  m_functions[funName] = boost::shared_ptr<Function>(funNew);
}

} // namespace Formula
