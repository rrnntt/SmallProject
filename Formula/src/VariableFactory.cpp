#include "Formula/VariableFactory.h"
#include "Kernel/Framework.h"

namespace Formula
{

  struct FormulaStuff
  {
    ~FormulaStuff()
    {
      std::cerr << "Formula unloaded\n";
    }
  };

  static FormulaStuff stuff;

  VariableFactory::~VariableFactory()
  {
    std::cerr << "VariableFactory deleted\n";
  }

  VariableFactory::VariableFactory()
  {
    std::cerr << "VariableFactory created\n";
  }

  VariableFactory& VariableFactory::instance()
  {
    try
    {
      Kernel::Singleton* s = Kernel::Framework::instance().getSingleton("VariableFactory");
      if (s == nullptr)
      {
        VariableFactory *vf = new VariableFactory();
        Kernel::Framework::instance().registerSingleton("VariableFactory",vf);
        return *vf;
      }
      else
      {
        return *static_cast<VariableFactory*>(s);
      }
    }
    catch(std::exception& e)
    {
      std::cerr << "VariableFactory failed " << e.what() << std::endl;
    }
  }

} // Formula
