#include "Numeric/MakeQuadratureScheme.h"
#include "Numeric/FunctionFactory.h"
#include "Numeric/IFunction.h"
#include "Numeric/ChebFunction.h"
#include "Numeric/Constants.h"
#include "Numeric/ChebfunWorkspace.h"

#include "API/AlgorithmFactory.h"
#include "API/TableWorkspace.h"
#include "API/TableColumn.h"
#include "API/WorkspaceFactory.h"

#include "Kernel/CommonProperties.h"
#include "Kernel/EParser.h"

#include <boost/lexical_cast.hpp>

namespace Numeric
{

DECLARE_ALGORITHM(MakeQuadratureScheme);

/// Constructor. Declare algorithm properties.
MakeQuadratureScheme::MakeQuadratureScheme()
{
  // number of quadrature points
  declareInt("N",24);
  // integration interval
  declareString("Interval","-1.0,1.0");
  // FunctionFcatory init string for a weight function
  declareString("Function");
  // TableWorkspace with the created qudrature data
  declareWorkspace("Quadrature");
  declareWorkspace("Test");
}

namespace 
{
  double xfun(double x){return x;}
}

/// Execute algorithm.
void MakeQuadratureScheme::exec()
{
  std::string intervalStr = get("Interval");
  Kernel::EParser intervalParser;
  intervalParser.parse( intervalStr );
  intervalParser.toList();
  if ( intervalParser.size() < 2 )
  {
    throw std::invalid_argument("Interval must have at least two bounds");
  }

  double startX = readDouble( intervalParser[0].str() );
  double endX = readDouble( intervalParser[1].str() );
  
  if (endX <= startX)
  {
    throw std::invalid_argument("StartX must be <= EndX");
  }

  ChebFunction weight(0, startX, endX);
  for(size_t i = 2; i < intervalParser.size(); ++i)
  {
    double x = readDouble( intervalParser[i].str() );
    weight.appendRight( 0, x ); // throws if x isn't right
  }

  int n = get("N");
  if (n < 2)
  {
    throw std::invalid_argument("N must be > 1");
  }
  std::string funStr = get("Function");
  IFunction_sptr fun = FunctionFactory::instance().createFitFunction(funStr);

  // fit ChebFunction 
  weight.bestFit( *fun );
  for(size_t i = 0; i < weight.nfuns(); ++i)
  {
    const ScaledChebfun& f = const_cast<const ChebFunction&>(weight).fun(i);
    std::cerr << i << ' ' << f.n() << std::endl;
  }
  std::cerr << "integr " << weight.integr() - sqrt(pi)/2 << std::endl;
  const ChebFunction& cweight = weight;

  // create the output table
  auto tws = API::TableWorkspace_ptr(dynamic_cast<API::TableWorkspace*>(
    API::WorkspaceFactory::instance().create("TableWorkspace"))
    );
  setProperty("Quadrature",tws);

  tws->addColumn("double","x");
  tws->getColumn("x")->asNumeric()->setPlotRole(API::NumericColumn::X);
  // a-coefficients in the recurrence relation p_j+1 = (x-a_j)*p_j - b_j*p_j-1
  tws->addColumn("double","a");
  // b-coefficients in the recurrence relation p_j+1 = (x-a_j)*p_j - b_j*p_j-1
  tws->addColumn("double","b");
  tws->setRowCount(n);

  std::vector<chebfun_sptr> poly( n );
  for(size_t i = 0; i < n; ++i)
  {
    poly[i] = chebfun_sptr( new chebfun(n, startX, endX) );
  }

  *poly[0] = 1.0;
  // !! it's not very good because it potentially can change the size of the column
  tws->getDoubleData("x") = poly[0]->getBase()->x;
  ChebFunction xx(cweight);
  xx.fit( xfun );
  ScaledChebfun xpp(n, startX, endX);
  ScaledChebfun pp(n, startX, endX);
  pp = *poly[0];
  pp *= *poly[0];
  pp *= cweight.fun(0);

  xpp = pp;
  xpp *= xx;

  auto test = ChebfunWorkspace_sptr( new ChebfunWorkspace(xpp) );
  setProperty("Test",test);
}

/**
 * Interpret a string and convert it into a double number.
 * @param str :: Number as a string. Possible aliases: inf for infinity, minf for negative infinity.
 */
double MakeQuadratureScheme::readDouble(const std::string& str) const
{
  if ( str == "inf" )
  {
    return inf;
  }
  else if ( str == "minf" )
  {
    return minf;
  }
  return boost::lexical_cast<double>( str );
}


} // namespace Numeric
