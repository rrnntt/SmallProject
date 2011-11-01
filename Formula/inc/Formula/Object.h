#ifndef FORMULA_OBJECT_H
#define FORMULA_OBJECT_H

#include "Formula/DllExport.h"
#include <string>
#include <iostream>
#include <boost/shared_ptr.hpp>

namespace Formula
{

class Variable;

class FORMULA_EXPORT Object
{
public:
  Object(){}
  Object(const std::string& oName):m_name(oName){}
  Object(const Object& o):m_name(o.m_name){}
  virtual std::string type()const = 0;
  const std::string& name()const{return m_name;}
  virtual void setName(const std::string& nm);
  virtual const Variable& operator()()const = 0;
  virtual const Variable& eval()const = 0;
private:
  std::string m_name;
};

inline void Object::setName(const std::string& nm)
{
  m_name = nm;
}

typedef boost::shared_ptr<Object> Object_ptr;
typedef boost::shared_ptr<const Object> Object_cptr;

} // Formula

#endif // FORMULA_OBJECT_H
