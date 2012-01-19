#ifndef DEMO_H
#define DEMO_H

#include "QtAPI/Task.h"

class Demo: public QtAPI::Task
{
public:
  virtual Type type() const{return Front;}
};

#endif /*DEMO_H*/