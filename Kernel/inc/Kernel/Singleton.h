#ifndef KERNEL_SINGLETON_H
#define KERNEL_SINGLETON_H

#include "Kernel/DllExport.h"

#include <string>

namespace Kernel
{

class KERNEL_EXPORT Singleton
{
public:
  virtual ~Singleton(){}
  virtual std::string name() const = 0;// {return "Singleton";}
protected:
  //Singleton();
};

template<class C>
C* returnSingleton(Singleton* s)
{
  if (s == nullptr)
  {
    C *new_s = new C();
    API::Framework::instance().registerSingleton(new_s->name(),new_s);
    return new_s;
  }
  else
  {
    return static_cast<C*>(s);
  }
}

} // Kernel

#endif // KERNEL_SINGLETON_H
