#ifndef API_SINGLETON_H
#define API_SINGLETON_H

#include "API/DllExport.h"

#include <string>

namespace API
{

class API_EXPORT Singleton
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

} // API

#endif // API_SINGLETON_H
