#ifndef DUMMYFUN_H
#define DUMMYFUN_H

#include "../Function.h"
#include "../Scalar.h"

using namespace Formula;

class AScalarFunction: public ReturnTypeFunction<Scalar>
{
public:
  const Variable& eval()const
  {
    m_return.setValue(0.0);
    for(size_t i=0;i<m_argv.size();++i)
    {
      m_return.plus(m_argv[i]().as<Scalar>());
    }
    return m_return;
  }
  int testArguments(std::vector<Argument>& args)
  {
    for(size_t i=0;i<args.size();++i)
    {
      if (args[i]().type() != "Scalar") return i+1;
    }
    return 0;
  }
};

#endif // DUMMYFUN_H
