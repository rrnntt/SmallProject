#include "Numeric/SolveODE.h"
#include "Numeric/ChebfunWorkspace.h"
#include "Numeric/ChebOperator.h"

#include "API/AlgorithmFactory.h"
#include "API/WorkspaceProperty.h"

#include "Kernel/CommonProperties.h"

namespace Numeric
{

DECLARE_ALGORITHM(SolveODE);

/// Constructor. Declare algorithm properties.
SolveODE::SolveODE()
{
  declare("Operator",new Kernel::StringProperty);
  declare("StartX",new Kernel::DoubleProperty);
  declare("EndX",new Kernel::DoubleProperty);
  declare("N",new Kernel::IntProperty);
  declare("Chebfun",new API::WorkspaceProperty(Kernel::Property::Output));
}

/// Execute algorithm.
void SolveODE::exec()
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
  std::string opStr = get("Operator");
  auto oper = ChebOperator::create( opStr );

  ChebfunWorkspace *cws = new ChebfunWorkspace;
  cws->fun(0).set(n,startX,endX);

  wsProp = boost::shared_ptr<ChebfunWorkspace>(cws);
}

} // namespace Numeric
