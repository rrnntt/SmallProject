#ifndef API_ALGORITHMFACTORY_H
#define API_ALGORITHMFACTORY_H

#include "Kernel/DynamicFactory.h"
#include "API/DllExport.h"
#include "API/Algorithm.h"
#include "API/Singleton.h"

namespace API
{

class API_EXPORT AlgorithmFactory: public Kernel::DynamicFactory<Algorithm>, protected Singleton
{
  AlgorithmFactory();
public:
  virtual std::string name() const {return "AlgorithmFactory";}
  static AlgorithmFactory& instance();
  boost::shared_ptr<Algorithm> createAlgorithm(const std::string& name) const;
};

} // API

#define DECLARE_ALGORITHM(type) \
int declare_algorithm_##type( (API::AlgorithmFactory::instance().subscribe<type>(#type),0));

#endif // API_ALGORITHMFACTORY_H
