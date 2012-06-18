#include "Numeric/CreateChebfun.h"
#include "Numeric/ChebfunWorkspace.h"

#include "Numeric/FunctionFactory.h"
#include "Numeric/IFunction.h"

#include "API/AlgorithmFactory.h"

#include "Kernel/CommonProperties.h"

namespace Numeric
{

DECLARE_ALGORITHM(CreateChebfun);

CreateChebfun::CreateChebfun()
{
  declareClass("Chebfun","WorkspaceManager");
  declareDouble("StartX",-1.0);
  declareDouble("EndX",1.0);
  declareInt("N",24);
  declareString("Function");
}

void CreateChebfun::exec()
{
  //API::WorkspaceProperty wsProp = get("Chebfun").as<API::WorkspaceProperty>();
  //std::string wsName = static_cast<std::string>(wsProp);

  double startX = get("StartX");
  double endX = get("EndX");
  if (endX <= startX)
  {
    throw std::invalid_argument("StartX must be <= EndX");
  }
  int n = get("N");
  if (n < 2)
  {
    throw std::invalid_argument("N must be > 1");
  }
  std::string funStr = get("Function");
  Numeric::IFunction_sptr fun = Numeric::FunctionFactory::instance().createFitFunction(funStr);

  ChebfunWorkspace *cws = new ChebfunWorkspace;
  cws->fun(0).set(n,startX,endX);
  cws->fun(0).fit(*fun);

  setProperty("Chebfun", boost::shared_ptr<ChebfunWorkspace>(cws));
}

} // namespace Numeric
