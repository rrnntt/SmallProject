#include "Numeric/CreateChebfun.h"
#include "Numeric/ChebfunWorkspace.h"

#include "Numeric/FunctionFactory.h"
#include "Numeric/IFunction.h"

#include "API/AlgorithmFactory.h"
#include "API/WorkspaceProperty.h"

#include "Kernel/CommonProperties.h"

namespace Numeric
{

DECLARE_ALGORITHM(CreateChebfun);

CreateChebfun::CreateChebfun()
{
  declare("Chebfun",new API::WorkspaceProperty(Kernel::Property::Output));
  declare("StartX",new Kernel::DoubleProperty);
  declare("EndX",new Kernel::DoubleProperty);
  declare("N",new Kernel::IntProperty);
  declare("Function",new Kernel::StringProperty);
}

void CreateChebfun::exec()
{
  API::WorkspaceProperty wsProp = get("Chebfun").as<API::WorkspaceProperty>();
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
  cws->fun(0).set(n,startX,endX);
  cws->fun(0).fit(*fun);

  wsProp = boost::shared_ptr<ChebfunWorkspace>(cws);
}

} // namespace Numeric
