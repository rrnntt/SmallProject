#include "Formula/VariableFactory.h"

namespace Formula
{

  VariableFactory& VariableFactory::instance()
  {
    static VariableFactory vf;
    return vf;
  }

} // Formula
