#ifndef DEMO_H
#define DEMO_H

#include "Task.h"

class Demo: public Task
{
public:
  virtual Type type() const{return Front;}
};

#endif /*DEMO_H*/