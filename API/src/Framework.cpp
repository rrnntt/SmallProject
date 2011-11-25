#include "API/Framework.h"

namespace API
{

Framework& Framework::instance()
{
  static Framework framework;
  return framework;
}

Framework::Framework()
{
}

} // API
