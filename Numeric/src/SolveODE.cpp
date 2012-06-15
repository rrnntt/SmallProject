#include "Numeric/SolveODE.h"
#include "Numeric/ChebfunWorkspace.h"
#include "Numeric/ChebOperator.h"

#include "API/AlgorithmFactory.h"

#include "Kernel/CommonProperties.h"
#include "Kernel/EParser.h"

#include <boost/lexical_cast.hpp>

namespace Numeric
{

DECLARE_ALGORITHM(SolveODE);

/// Constructor. Declare algorithm properties.
SolveODE::SolveODE()
{
  declareString("Operator");
  std::vector<std::string> boundary;
  boundary.push_back("Dirichlet");
  boundary.push_back("Cauchy");
  declareString("BoundaryConditions");
  declareString("BoundaryValues");
  declareDouble("StartX");
  declareDouble("EndX");
  declareInt("N");
  declareClass("Chebfun","WorkspaceManager");
}

/// Execute algorithm.
void SolveODE::exec()
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
  std::string opStr = get("Operator");
  auto oper = ChebOperator::create( opStr );

  std::string boundary = get("BoundaryConditions");
  std::string boundaryValues = get("BoundaryValues");
  Kernel::EParser parser;
  parser.parse(boundaryValues);
  parser.toList();
  std::vector<double> bValues;
  for(size_t i = 0; i < parser.size(); ++i)
  {
    double d = boost::lexical_cast<double>( parser[i].name() );
    bValues.push_back( d );
  }

  BoundaryConditions* bc = nullptr;
  std::cerr << '<' << boundary << '>' << std::endl;
  if ( boundary == "Dirichlet" )
  {
    if ( bValues.size() == 1 )
    {
      bc = new Dirichlet( bValues[0] );
    }
    else if ( bValues.size() == 2 )
    {
      bc = new Dirichlet( bValues[0], bValues[1] );
    }
    else
    {
      throw std::invalid_argument("Dirichlet boundary conditions must have 1 or 2 values");
    }
  }
  else if ( boundary == "Cauchy" )
  {
    if ( bValues.size() == 2 )
    {
      bc = new Cauchy( bValues[0], bValues[1] );
    }
    else
    {
      throw std::invalid_argument("Cauchy boundary conditions must have 2 values");
    }
  }
  else
  {
    throw std::invalid_argument("Unknown boundary conditions: Dirichlet or Cauchy are allowed");
  }

  ChebfunWorkspace *cws = new ChebfunWorkspace;
  cws->fun(0).set(n,startX,endX);

  oper->log();
  oper->solve(cws->fun(0), *bc);

  setProperty("Chebfun", boost::shared_ptr<ChebfunWorkspace>(cws));
}

} // namespace Numeric
