#include "Formula/Reference.h"
#include "Formula/VariableFactory.h"

namespace Formula
{

DECLARE_VARIABLE(Address);

Reference::Reference():
    Variable()
{

}

Reference::Reference(Variable_ptr var)
  :m_var(var)
{

}

Reference::Reference(const Reference& ref):
    Variable(ref.name()),m_var(ref.m_var)
{

}

std::string Reference::type() const
{
  if (m_var)
  {
    return m_var->type();
  }
  return "Reference";
}

void Reference::setName(const std::string& nm)
{
  if (m_var)
  {
    m_var->setName(nm);
  }
}

const Variable& Reference::operator()()const
{
  if (m_var)
  {
    return (*m_var)();
  }
  throw std::runtime_error("Uninitialized reference");
}

const Variable& Reference::eval()const
{
  if (m_var)
  {
    return m_var->eval();
  }
  throw std::runtime_error("Uninitialized reference");
}

std::string Reference::toString()const
{
  if (m_var)
  {
    return m_var->toString();
  }
  return "Uninitialized_reference";
}

bool Reference::setValue(const std::string& str)
{
  if (m_var)
  {
    return m_var->setValue(str);
  }
  return false;
}

bool Reference::setValue(const Variable& v)
{
  //if ()
  if (m_var)
  {
    return m_var->setValue(v);
  }
  return false;
}

bool Reference::setValue(const double* d)
{
  if (m_var)
  {
    return m_var->setValue(d);
  }
  return false;
}

void Reference::setRandomValue()
{
  if (m_var)
  {
    m_var->setRandomValue();
  }
}

Variable* Reference::clone()const
{
  return new Reference(*this);
}

bool Reference::operator==(const Variable& v)
{
  if (m_var)
  {
    return m_var->operator ==(v);
  }
  throw std::runtime_error("Uninitialized reference");
}

bool Reference::operator!=(const Variable& v)
{
  if (m_var)
  {
    return m_var->operator !=(v);
  }
  throw std::runtime_error("Uninitialized reference");
}

bool Reference::compare(const Variable& v,double absTol, double relTol)
{
  if (m_var)
  {
    return m_var->compare(v,absTol,relTol);
  }
}

bool Reference::isLiteral(const std::string& str)const
{
  if (m_var)
  {
    return m_var->isLiteral(str);
  }
  return false;
}

void Reference::reset(Variable_ptr var)
{
  m_var = var;
}

} // Formula
