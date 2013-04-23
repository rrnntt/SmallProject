#include "Formula/Bool.h"
#include "Formula/VariableFactory.h"
#include "Formula/FunctionFactory.h"
#include "Formula/Function.h"

#include <algorithm>
#include <cctype>

namespace Formula
{

  DECLARE_VARIABLE(Bool);

  Bool::Bool():m_value(){}
  Bool::Bool(const Bool& b):m_value(b.m_value){}

  std::string Bool::toString()const
  {
    return m_value ? "True" : "False";
  }

  bool Bool::setValue(const std::string& in)
  {
    std::string read(in);
    std::transform(in.begin(),in.end(),read.begin(),toupper);
    if (read == "TRUE")
    {
      m_value = true;
    }
    else
    {
      m_value = false;
    }
    return true;
  }

  bool Bool::setValue(const Variable& var)
  {
    const Bool* b = dynamic_cast<const Bool*>(&var);
    if (!b) return false;
    m_value = b->m_value;
    return true;
  }

  bool Bool::setValue(const double* d)
  {
    m_value = (*d != 0.0);
    return true;
  }

  void Bool::setRandomValue()
  {
    m_value = (rand() > RAND_MAX/2);
  }

  Variable* Bool::clone()const
  {
    return new Bool(*this);
  }

  bool Bool::operator==(const Variable& var)
  {
    const Bool* b = dynamic_cast<const Bool*>(&var);
    if (!b)
    {
      throw std::runtime_error("Cannot compare Bool and "+var.type());
    }
    return (m_value == b->m_value);
  }

  bool Bool::operator!=(const Variable& var)
  {
    return !(*this == var);
  }

  bool Bool::compare(const Variable& var,double, double)
  {
    return (*this == var);
  }

  bool Bool::isLiteral(const std::string& in)const
  {
    std::string read(in);
    std::transform(in.begin(),in.end(),read.begin(),toupper);
    return (read == "TRUE" || read == "FALSE");
  }

  //---------     Bool methods    ----------//

  void Bool::setValue(bool b)
  {
    m_value = b;
  }

  void Bool::And(const Bool& b)
  {
    m_value = m_value && b.m_value;
  }

  void Bool::Or(const Bool& b)
  {
    m_value = m_value || b.m_value;
  }


  //------ Define Bool Functions --------//

  class BoolFunction: public ReturnTypeFunction<Bool>
  {
  public:
    int testArguments(std::vector<Argument>& args)
    {
      for(size_t i=0;i<args.size();++i)
      {
        if (args[i]().type() != "Bool") return i+1;
      }
      return 0;
    }
  };

  class AndFunction: public BoolFunction
  {
  public:
    std::string type()const{return "AndFunction";}
    const Variable& eval()const
    {
       m_return.setValue(true);
      for(size_t i=0;i<m_argv.size();++i)
      {
          m_return.And(m_argv[i]().as<Bool>());
      }
      return m_return;
    }
  };

  DECLARE_FUNCTION(AndFunction,"&","Bool,...");

  class OrFunction: public BoolFunction
  {
  public:
    std::string type()const{return "OrFunction";}
    const Variable& eval()const
    {
      m_return.setValue(false);
      for(size_t i=0;i<m_argv.size();++i)
      {
          m_return.Or(m_argv[i]().as<Bool>());
      }
      return m_return;
    }
  };

  DECLARE_FUNCTION(OrFunction,"|","Bool,...");



} // Formula
