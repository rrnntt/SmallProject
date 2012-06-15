#include "API/AlgorithmFactory.h"
#include "Kernel/Framework.h"

namespace API
{

AlgorithmFactory::AlgorithmFactory()
{
}

AlgorithmFactory& AlgorithmFactory::instance()
{
  Singleton* s = Kernel::Framework::instance().getSingleton("AlgorithmFactory");
  if (s == nullptr)
  {
    AlgorithmFactory *f = new AlgorithmFactory();
    Kernel::Framework::instance().registerSingleton("AlgorithmFactory",f);
    return *f;
  }
  else
  {
    return *static_cast<AlgorithmFactory*>(s);
  }
}

boost::shared_ptr<Algorithm> AlgorithmFactory::createAlgorithm(const std::string& name) const
{
  return Algorithm_ptr(create(name));
}

} // API
