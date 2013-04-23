#ifndef SINGLETONHOLDER_H
#define SINGLETONHOLDER_H

namespace Kernel
{

template<class C>
class SingletonHolder
{
public:
  static C& instance()
  {
    static C c;
    return c;
  }
};

} // Kernel

#endif // SINGLETONHOLDER_H
