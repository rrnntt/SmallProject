#ifndef KERNEL_CREATOR_H
#define KERNEL_CREATOR_H

namespace Kernel
{
  template<class Base>
  class AbstractCreator
  {
  public:
    virtual Base* create() = 0;
  };

  template<class C,class Base>
  class Creator: public AbstractCreator<Base>
  {
  public:
    Base* create(){return static_cast<Base*>(new C);}
  };

} // Kernel

#endif // KERNEL_CREATOR_H
