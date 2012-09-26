#ifndef GOBLIN_TESTMORSE_H
#define GOBLIN_TESTMORSE_H

#include "Goblin/DllExport.h"
#include "API/Algorithm.h"

namespace Goblin
{

class GOBLIN_EXPORT TestMorse: public API::Algorithm
{
public:
  TestMorse();
  virtual std::string name() const {return "TestMorse";}
protected:
  virtual void exec();
};

} // GOBLIN

#endif // GOBLIN_TESTMORSE_H
