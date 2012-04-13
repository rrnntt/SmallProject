#include "Formula/Scalar.h"
#include "Formula/Expression.h"
#include "Formula/VariableFactory.h"
#include "Formula/FunctionFactory.h"
#include "Formula/Bool.h"

#include <iostream>
#include <sstream>
#include <cmath>

namespace Formula
{

  DECLARE_VARIABLE(Scalar);

Scalar::Scalar()
:Variable(),m_store_value(),m_value(m_store_value)
{
    //std::cerr<<"Scalar created\n";
}

/**
  * Create a Scalar with value equal to the value of a C++ double
  */
Scalar::Scalar(const double& d)
:Variable(),m_store_value(d),m_value(m_store_value)
{
}

/**
  * Copy constructor.
  */
Scalar::Scalar(const Scalar& sc)
:Variable(),m_store_value(sc.m_value),m_value(m_store_value)
{
}

/**
  * Create a Scalar which is a reference to a C++ double variable
  */
Scalar::Scalar(double* d)
:Variable(),m_store_value(*d),m_value(*d)
{
}

Scalar::~Scalar()
{
}

std::string Scalar::toString()const
{
  std::ostringstream ostr;
  ostr << m_value;
  return ostr.str();
}

Variable* Scalar::clone()const
{
    return new Scalar(*this);
}

bool Scalar::isLiteral(const std::string& nm)const
{
  return Namespace::isNumber(nm);
}

bool Scalar::setValue(const std::string& val)
{
  std::istringstream istr(val);
  istr >> m_value;
  return true; //TODO throw exception when failed
}

bool Scalar::setValue(const Variable& o)
{
  const Scalar* s = dynamic_cast<const Scalar*>(&o);
  if (!s) return false;
  m_value = s->m_value;
  return true; //TODO throw exception when failed
}

bool Scalar::setValue(const double* d)
{
  m_value = *d;
  return true;
}

bool Scalar::setValue(const double& d)
{
  m_value = d;
  return true;
}

void Scalar::setRandomValue()
{
  m_value = double(rand())/RAND_MAX;
}

void Scalar::negate()
{
  m_value = -m_value;
}

void Scalar::negate(const Scalar& s)
{
  m_value = - s.m_value;
}

void Scalar::plus(const Scalar& s)
{
  m_value += s.m_value;
}

void Scalar::minus(const Scalar& s)
{
  m_value -= s.m_value;
}

void Scalar::times(const Scalar& s)
{
  m_value *= s.m_value;
}

void Scalar::divide(const Scalar& s)
{
  m_value /= s.m_value;
}

void Scalar::power(const Scalar& s)
{
  m_value = pow(m_value,s.m_value);
}

bool Scalar::operator==(const Variable& o)
{
  const Scalar* s = dynamic_cast<const Scalar*>(&o);
  if (!s)
  {
    throw std::runtime_error("Cannot compare scalar with non-scalar");
  }
  return m_value == s->m_value;
}

bool Scalar::compare(const Variable& o,double absTol, double relTol)
{
  const Scalar* s = dynamic_cast<const Scalar*>(&o);
  if (!s)
  {
    throw std::runtime_error("Cannot compare scalar with non-scalar");
  }
  double x1 = m_value;
  double x2 = s->m_value;

  bool absOK = absTol < 0;
  bool relOK = relTol < 0;

  if (absOK && relOK)
  {
    return m_value == s->m_value;
  }

  if (!absOK)
  {
    absOK = fabs(x1 - x2) <= absTol;
  }

  if (!relOK)
  {
    if (x2 == 0) return x1 == 0;
    relOK = fabs(1. - x1/x2) <= relTol;
  }

  return absOK && relOK;
}

void Scalar::setExpression(boost::shared_ptr<Expression> expr)
{
  Scalar_ptr x = boost::dynamic_pointer_cast<Scalar>(expr->object());
  if (x)
  {
    if (Namespace::isNumber(x->name()))
    {
      setValue(*x.get());
    }
    else
    {
      Variable::setExpression(expr);
    }
  }
  else
  {
    Variable::setExpression(expr);
  }
}

//------ Define Scalar Functions --------//

class ScalarFunction: public ReturnTypeFunction<Scalar>
{
public:
  int testArguments(std::vector<Argument>& args)
  {
    for(size_t i=0;i<args.size();++i)
    {
      if (args[i]().type() != "Scalar") return i+1;
    }
    return 0;
  }
};

class ScalarFunction1Arg: public ReturnTypeFunction<Scalar>
{
public:
  int testArguments(std::vector<Argument>& args)
  {
    if (args.size() != 1) return 1;
    if (args[0]().type() != "Scalar") return 1;
    return 0;
  }
};

class PlusFunction: public ScalarFunction
{
public:
  std::string type()const{return "PlusFunction";}
  const Variable& eval()const
  {
    m_return.setValue(0.0);
    for(size_t i=0;i<m_argv.size();++i)
    {
      if (m_argv[i].inv)
      {
        m_return.minus(m_argv[i]().as<Scalar>());
      }
      else
      {
        m_return.plus(m_argv[i]().as<Scalar>());
      }
    }
    return m_return;
  }
  int get_inv(const std::string& op)const
  {
    return op == "-"? 1 : 0;
  }
};

DECLARE_FUNCTION(PlusFunction,"+","Scalar,...");

class TimesFunction: public ScalarFunction
{
public:
  std::string type()const{return "TimesFunction";}
  const Variable& eval()const
  {
    m_return.setValue(1);
    for(size_t i=0;i<m_argv.size();++i)
    {
      const Argument& arg = m_argv[i];
      if (arg.inv)
      {
        if (arg().as<Scalar>() == 0)
        {
          throw std::runtime_error("Division by zero");
        }
        m_return.divide(arg().as<Scalar>());
      }
      else
      {
        m_return.times(arg().as<Scalar>());
      }
    }
    return m_return;
  }
  int get_inv(const std::string& op)const
  {
    return op == "/"? 1 : 0;
  }
};

DECLARE_FUNCTION(TimesFunction,"*","Scalar,...");

class UnaryMinusFunction: public ScalarFunction1Arg
{
public:
  std::string type()const{return "UnaryMinusFunction";}
  const Variable& eval()const
  {
    m_return.negate(m_argv[0]().as<Scalar>());
    return m_return;
  }
};

DECLARE_FUNCTION(UnaryMinusFunction,"-","Scalar");

class PowerFunction: public ScalarFunction
{
public:
  std::string type()const{return "PowerFunction";}
  const Variable& eval()const
  {
    m_return.setValue(m_argv[0]().as<Scalar>());
    for(size_t i=1;i<m_argv.size();++i)
    {
      m_return.power(m_argv[i]().as<Scalar>());
    }
    return m_return;
  }
  int testArguments(std::vector<Argument>& args)
  {
    if (args.size() < 2) return 1;
    return ScalarFunction::testArguments(args);
  }
};
DECLARE_FUNCTION(PowerFunction,"^","Scalar,...");

class SineFunction: public ScalarFunction1Arg
{
public:
  std::string type()const{return "SineFunction";}
  const Variable& eval()const
  {
    double arg = m_argv[0]().as<Scalar>();
    m_return.setValue(sin(arg));
    return m_return;
  }
};
DECLARE_FUNCTION(SineFunction,"sin","Scalar");

class CosineFunction: public ScalarFunction1Arg
{
public:
  std::string type()const{return "CosineFunction";}
  const Variable& eval()const
  {
    double arg = m_argv[0]().as<Scalar>();
    m_return.setValue(cos(arg));
    return m_return;
  }
};
DECLARE_FUNCTION(CosineFunction,"cos","Scalar");

class TangentFunction: public ScalarFunction1Arg
{
public:
  std::string type()const{return "TangentFunction";}
  const Variable& eval()const
  {
    double arg = m_argv[0]().as<Scalar>();
    m_return.setValue(tan(arg));
    return m_return;
  }
};
DECLARE_FUNCTION(TangentFunction,"tan","Scalar");
typedef TangentFunction TangentFunction1;
DECLARE_FUNCTION(TangentFunction1,"tg","Scalar");

class CoTangentFunction: public ScalarFunction1Arg
{
public:
  std::string type()const{return "CoTangentFunction";}
  const Variable& eval()const
  {
    double arg = m_argv[0]().as<Scalar>();
    m_return.setValue(1./tan(arg));
    return m_return;
  }
};
DECLARE_FUNCTION(CoTangentFunction,"cot","Scalar");

class SecFunction: public ScalarFunction1Arg
{
public:
  std::string type()const{return "SecFunction";}
  const Variable& eval()const
  {
    double arg = m_argv[0]().as<Scalar>();
    m_return.setValue(1./cos(arg));
    return m_return;
  }
};
DECLARE_FUNCTION(SecFunction,"sec","Scalar");

class CoSecFunction: public ScalarFunction1Arg
{
public:
  std::string type()const{return "CoSecFunction";}
  const Variable& eval()const
  {
    double arg = m_argv[0]().as<Scalar>();
    m_return.setValue(1./sin(arg));
    return m_return;
  }
};
DECLARE_FUNCTION(CoSecFunction,"cosec","Scalar");

class SqrtFunction: public ScalarFunction1Arg
{
public:
  std::string type()const{return "SqrtFunction";}
  const Variable& eval()const
  {
    double arg = m_argv[0]().as<Scalar>();
    m_return.setValue(sqrt(arg));
    return m_return;
  }
};
DECLARE_FUNCTION(SqrtFunction,"sqrt","Scalar");

class LogFunction: public ScalarFunction1Arg
{
public:
  std::string type()const{return "LogFunction";}
  const Variable& eval()const
  {
    double arg = m_argv[0]().as<Scalar>();
    m_return.setValue(log10(arg));
    return m_return;
  }
};
DECLARE_FUNCTION(LogFunction,"log","Scalar");

class ExpFunction: public ScalarFunction1Arg
{
public:
  std::string type()const{return "ExpFunction";}
  const Variable& eval()const
  {
    double arg = m_argv[0]().as<Scalar>();
    m_return.setValue(exp(arg));
    return m_return;
  }
};
DECLARE_FUNCTION(ExpFunction,"exp","Scalar");

class ScalarCompareFunction: public ReturnTypeFunction<Bool>
{// "= != > >= < <="
public:
  int testArguments(std::vector<Argument>& args)
  {
    if (args.size() < 2) return 1;
    for(size_t i=0;i<args.size();++i)
    {
      if (args[i]().type() != "Scalar") return i+1;
    }
    return 0;
  }
  std::string type()const{return "ScalarCompareFunction";}
  const Variable& eval()const
  {
    double last = m_argv[0]().as<Scalar>();
    for(size_t i=1;i<m_argv.size();++i)
    {
      double current = m_argv[i]().as<Scalar>();
      bool ok;
      int fun = m_argv[i].inv;
      if (fun == 0)
      {
        ok = (last == current);
      }
      else if (fun == 1)
      {
        ok = (last != current);
      }
      else if (fun == 2)
      {
        ok = (last > current);
      }
      else if (fun == 3)
      {
        ok = (last >= current);
      }
      else if (fun == 4)
      {
        ok = (last < current);
      }
      else if (fun == 5)
      {
        ok = (last <= current);
      }
      m_return.setValue(ok);
    }
    return m_return;
  }
  int get_inv(const std::string& op)const
  {
    if (op == "=") return 0;
    else if (op == "!=") return 1;
    else if (op == ">") return 2;
    else if (op == ">=") return 3;
    else if (op == "<") return 4;
    else if (op == "<=") return 5;
  }
};

DECLARE_FUNCTION(ScalarCompareFunction,"=","Scalar,...");

} // Formula
