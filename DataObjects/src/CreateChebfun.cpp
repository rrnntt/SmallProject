#include "DataObjects/CreateChebfun.h"
#include "DataObjects/ChebfunWorkspace.h"

#include "Numeric/FunctionFactory.h"
#include "Numeric/IFunction.h"

#include "API/AlgorithmFactory.h"
#include "API/WorkspaceProperty.h"

#include "Kernel/CommonProperties.h"

namespace DataObjects
{

DECLARE_ALGORITHM(CreateChebfun);

CreateChebfun::CreateChebfun()
{
  declare("OutputWorkspace",new API::WorkspaceProperty(Kernel::Property::Output));
  declare("StartX",new Kernel::DoubleProperty);
  declare("EndX",new Kernel::DoubleProperty);
  declare("N",new Kernel::IntProperty);
  declare("Function",new Kernel::StringProperty);
}

void CreateChebfun::exec()
{
  API::WorkspaceProperty wsProp = get("OutputWorkspace").as<API::WorkspaceProperty>();
  std::string wsName = static_cast<std::string>(wsProp);

  double startX = get("StartX").to<double>();
  double endX = get("EndX").to<double>();
  if (endX <= startX)
  {
    throw std::invalid_argument("StartX must be <= EndX");
  }
  int n = get("N").to<int>();
  if (n < 2)
  {
    throw std::invalid_argument("N must be > 1");
  }
  std::string funStr = get("Function");
  Numeric::IFunction_sptr fun = Numeric::FunctionFactory::instance().createFitFunction(funStr);

  ChebfunWorkspace *cws = new ChebfunWorkspace;
  cws->set(n,startX,endX);
  cws->fun().fit(*fun);

  wsProp = boost::shared_ptr<ChebfunWorkspace>(cws);
}

} // namespace DataObjects
