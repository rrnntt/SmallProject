#include "Formula/Function.h"

namespace Formula
{
  void Function::setArguments(std::vector<Argument>& args)
  {
    m_argv.assign(args.begin(),args.end());
  }

} // Formula
