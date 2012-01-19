#ifndef GOBLIN_MIO_H
#define GOBLIN_MIO_H

#include "Goblin/DllExport.h"

#include <iostream>

namespace Goblin
{

class GOBLIN_EXPORT Mio
{
public:
  template<typename T>
  Mio& operator<<(const T& d)
  {
    std::cerr << d;
  }
};

extern Mio mio;

} // namespace Goblin
#endif // GOBLIN_MIO_H
