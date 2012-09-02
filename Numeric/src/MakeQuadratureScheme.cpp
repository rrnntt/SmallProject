#include "Numeric/MakeQuadratureScheme.h"
#include "Numeric/FunctionFactory.h"
#include "Numeric/IFunction.h"
#include "Numeric/IFunction1D.h"
#include "Numeric/ParamFunction.h"
#include "Numeric/ChebFunction.h"
#include "Numeric/Constants.h"
#include "Numeric/FunctionDomain1D.h"
#include "Numeric/FunctionValues.h"

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
  // guess start of the interval containing all integration points
  declareDouble("StartX",-1);
  // guess end of the interval containing all integration points
  declareDouble("EndX",1);
  declareWorkspace("Test");
}

namespace 
{
  /// function x
  double xfun(double x){return x;}
  /// add function values to a table
  void addValuesToWorkspace(API::TableWorkspace_ptr tws, 
    const std::string& xColumn,
    const std::string& yColumn,
    const IFunction& fun);
  /// valuate a polynom recursively
  double evalPoly(size_t n, double x, const std::vector<double>& a, const std::vector<double>& b);
  /// valuate a polynom recursively
  class EvalPoly: public IFunction1D, public ParamFunction
  {
    const size_t m_n;
    const std::vector<double>& m_a;
    const std::vector<double>& m_b;
  public:
    EvalPoly(size_t n, const std::vector<double>& a, const std::vector<double>& b):
    m_n(n),m_a(a),m_b(b)
    {}
    /// Returns the function's name
    virtual std::string name()const {return "EvalPoly";}
    /// Function you want to fit to.
    virtual void function1D(double* out, const double* xValues, const size_t nData)const
    {
      for(size_t i = 0; i < nData; ++i)
      {
        out[i] = evalPoly(m_n, xValues[i], m_a, m_b);
      }
    }
  };
}

/// Execute algorithm.
void MakeQuadratureScheme::exec()
{
  std::string intervalStr = get("Interval");
  Kernel::EParser intervalParser;
  intervalParser.parse( intervalStr );
  intervalParser.toList();
  intervalParser.log();
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

  startX = get("StartX");
  endX = get("EndX");

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
  // we need the root of it
  try
  {
    weight.sqrt();
  }
  catch(...)
  {
    throw std::runtime_error("Weight function must be positive everywhere");
  }

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
  auto& a = tws->getDoubleData("a");
  auto& b = tws->getDoubleData("b");

  {
    FunctionDomain1DVector xdomain(startX, endX, n);
    // !! it's not very good because it potentially can change the size of the column
    tws->getDoubleData("x").assign(xdomain.getPointerAt(0),xdomain.getPointerAt(0) + n);
  }

  auto test = API::TableWorkspace_ptr( new API::TableWorkspace );
  test->addColumn("double","x");
  test->getColumn("x")->asNumeric()->setPlotRole(API::NumericColumn::X);
  const size_t testSize = 200;
  test->setRowCount(testSize);
  {
    FunctionDomain1DVector xdomain(startX, endX, testSize);
    // !! it's not very good because it potentially can change the size of the column
    test->getDoubleData("x").assign(xdomain.getPointerAt(0),xdomain.getPointerAt(0) + testSize);
  }
  setProperty("Test",test);

  std::vector<ChebFunction_sptr> poly( n );
  for(size_t i = 0; i < n; ++i)
  {
    poly[i] = ChebFunction_sptr( new ChebFunction(weight, false) );
  }

  std::vector<double> norms( n );

  *poly[0] = weight;

  // don't copy just use the same base
  ChebFunction pp(*poly[0]);
  pp *= *poly[0];
  norms[0] = pp.integr();
  pp *= xfun;
  a[0] = pp.integr() / norms[0];
  b[0] = 0.0;

  *poly[1] = *poly[0];
  *poly[1] *= xfun;
  pp = *poly[0];
  pp *= a[0];
  *poly[1] -= pp;

  pp = *poly[1];
  pp *= *poly[0];
  std::cerr << "integr " << pp.integr() << std::endl;

  addValuesToWorkspace( test, "x", "p0", *poly[0] );
  addValuesToWorkspace( test, "x", "p1", *poly[1] );

  for(size_t i = 2; i < n; ++i)
  {
    std::cerr << "making " << i << "-th poly" << std::endl;
    size_t i1 = i - 1;
    size_t i2 = i - 2;
    // calculate a and b for this iteration
    pp = *poly[i1];
    pp *= *poly[i1];
    norms[i1] = pp.integr();
    pp *= xfun;
    a[i1] = pp.integr() / norms[i1];
    b[i1] = norms[i1] / norms[i2];

    //calculate i-th poly
    *poly[i] = *poly[i1];
    *poly[i] *= xfun;
    pp = *poly[i1];
    pp *= a[i1];
    *poly[i] -= pp;
    pp = *poly[i2];
    pp *= b[i1];
    *poly[i] -= pp;
    addValuesToWorkspace( test, "x", "p"+boost::lexical_cast<std::string>(i), *poly[i] );
  }

  //std::cerr << "Calculating abscissas" << std::endl;
  //EvalPoly ePoly(n, a, b);
  //ChebFunction res(n, startX, endX);
  //res.bestFit( *poly.back() );
  //addValuesToWorkspace( test, "x", "res", res );
  //std::vector<double> r;
  //res.roots( r );
  //std::cerr << "size " << r.size() << std::endl;

  //for(size_t i = 0; i < r.size(); ++i)
  //{
  //  std::cerr << i << ' ' << r[i] << std::endl;
  //}
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

namespace 
{
  /**
   * Add a column to a table workspace filled with double values
   * calculated with a function wich uses another numeric column as its x values.
   * @param tws :: The table workspace.
   * @param xColumn :: Name of the column with the x-values. Must exist and be numeric
   * @param yColumn :: Name of a column to store the calculated values. If a column with 
   *  this name exists it will be overwritten if numeric or exception thrown otherwise.
   *  If the column doesn't exist it will be created.
   * @param fun :: A function to calculate the values.
   */
  void addValuesToWorkspace(API::TableWorkspace_ptr tws, 
    const std::string& xColumn,
    const std::string& yColumn,
    const IFunction& fun)
  {
    auto& x = tws->getDoubleData( xColumn );
    if ( tws->hasColumn( yColumn ) )
    {
      if ( !tws->getColumn( yColumn )->isNumeric() )
        throw std::runtime_error("Column "+yColumn+" isn't numeric");
    }
    else
    {
      tws->addColumn( "double", yColumn );
    }
    tws->getColumn( yColumn )->asNumeric()->setPlotRole(API::NumericColumn::Y);
    auto& y = tws->getDoubleData( yColumn );

    FunctionDomain1DView domain( x.data(), x.size() );
    FunctionValues values( domain );
    fun.function( domain, values );
    for(size_t i = 0; i < y.size(); ++i)
    {
      y[i] = values.getCalculated(i);
    }
  }

  /**
   * Evaluate a polynom recursively: 
   * @param n :: Polynom order
   * @param x :: Abscissa at wich to evaluate the polynom.
   * @param a :: The a-coefficients of the recurrence.
   * @param b :: The b-coefficients of the recurrence.
   */
  double evalPoly(size_t n, double x, const std::vector<double>& a, const std::vector<double>& b)
  {
    double p0 = 1.0;
    if ( n == 0 ) return p0;
    double p1 = ( x - a[0] ) * p0;
    if ( n == 1 ) return p1;
    for(size_t i = 2; i <= n; ++i)
    {
      double p = ( x - a[i-1] ) * p1 - b[i-1] * p0;
      p0 = p1;
      p1 = p;
    }
    return p1;
  }

}


} // namespace Numeric
