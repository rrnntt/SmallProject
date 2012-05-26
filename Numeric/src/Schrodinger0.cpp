#include "Numeric/Schrodinger0.h"
#include "Numeric/ChebfunWorkspace.h"
#include "Numeric/ChebOperator.h"

#include "API/AlgorithmFactory.h"
#include "API/WorkspaceProperty.h"

#include "Kernel/CommonProperties.h"
#include "Kernel/EParser.h"

#include <boost/lexical_cast.hpp>

namespace Numeric
{

DECLARE_ALGORITHM(Schrodinger0);

/// Constructor. Declare algorithm properties.
Schrodinger0::Schrodinger0()
{
  declare("Operator",new Kernel::StringProperty);
  std::vector<std::string> boundary;
  //boundary.push_back("Dirichlet");
  //boundary.push_back("Cauchy");
  //declare("BoundaryConditions",new Kernel::StringProperty(boundary));
  declare("BoundaryValues",new Kernel::StringProperty);
  declare("StartX",new Kernel::DoubleProperty);
  declare("EndX",new Kernel::DoubleProperty);
  declare("N",new Kernel::IntProperty);
  declare("Chebfun",new API::WorkspaceProperty(Kernel::Property::Output));
}

/// Execute algorithm.
void Schrodinger0::exec()
{
  API::WorkspaceProperty wsProp = get("Chebfun").as<API::WorkspaceProperty>();
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
  auto& y = cws->fun(0);
  y.set(n,startX,endX);

  GSLMatrix L;
  //size_t n = y.n() + 1;
  oper->createMatrix( y.getBase(), L );

  GSLVector d;
  GSLMatrix v;
  L.diag( d, v );

  size_t imin = 0;
  double dmin = d[0];
  for(size_t i = 1; i < d.size(); ++i)
  {
    const double tmp = d[i];
    if ( tmp < dmin )
    {
      dmin = tmp;
      imin = i;
    }
  }

  y.setP( v, imin );

  wsProp = boost::shared_ptr<ChebfunWorkspace>(cws);
}

} // namespace Numeric
