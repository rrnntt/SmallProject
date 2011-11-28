#include "Formula/VariableFactory.h"
#include "API/Framework.h"

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
      API::Singleton* s = API::Framework::instance().getSingleton("VariableFactory");
      if (s == nullptr)
      {
        VariableFactory *vf = new VariableFactory();
        API::Framework::instance().registerSingleton("VariableFactory",vf);
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
