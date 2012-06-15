#include "Formula/FunctionFactory.h"
#include "Kernel/Framework.h"

namespace Formula
{
  FunctionFactory::FunctionFactory()
  {
  }

  FunctionFactory& FunctionFactory::instance()
  {
    Kernel::Singleton* s = Kernel::Framework::instance().getSingleton("Formula::FunctionFactory");
    if (s == nullptr)
    {
      FunctionFactory *vf = new FunctionFactory();
      Kernel::Framework::instance().registerSingleton("Formula::FunctionFactory",vf);
      return *vf;
    }
    else
    {
      return *static_cast<FunctionFactory*>(s);
    }
  }

  /**
    * Create a function with name fun and arguments args.
    * @param fun The function name
    * @param args The function arguments.
    * @return Pointer to the created function or NULL if it cannot be created
    */
  Function* FunctionFactory::create(const std::string& fun,std::vector<Argument>& args)
  {
    std::string ini_str(fun);
    std::string last_type;
    int i_start_var = 0;
    for(int i=0;i<args.size();++i)
    {
      std::string type = args[i]().type();
      if (type == last_type)
      {
        if (i_start_var == 0)
        {
          i_start_var = ini_str.size()+1;
        }
      }
      else
      {
        last_type = type;
        i_start_var = 0;
      }
      ini_str += "," + type;
    }
    if (exists(ini_str))
    {
      return Kernel::DynamicFactory<Function>::create(ini_str);
    }
    else if (i_start_var > 0 && i_start_var < ini_str.size())
    {
      ini_str.replace(i_start_var,std::string::npos,"...");
      if (exists(ini_str))
      {
        return Kernel::DynamicFactory<Function>::create(ini_str);
      }
    }

    return NULL;
  }

} // Formula

