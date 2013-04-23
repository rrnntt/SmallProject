#include "Numeric/MakeQuadratureScheme.h"
#include "Numeric/FunctionFactory.h"
#include "Numeric/IFunction.h"
#include "Numeric/IFunction1D.h"
#include "Numeric/ParamFunction.h"
#include "Numeric/ChebFunction.h"
#include "Numeric/Constants.h"
#include "Numeric/FunctionDomain1D.h"
#include "Numeric/FunctionValues.h"
#include "Numeric/GSLMatrix.h"
#include "Numeric/GSLVector.h"
#include "Numeric/ChebfunVector.h"

#include "API/AlgorithmFactory.h"
#include "API/TableWorkspace.h"
#include "API/TableColumn.h"
#include "API/WorkspaceFactory.h"

#include "Kernel/CommonProperties.h"
#include "Kernel/EParser.h"

#include <boost/lexical_cast.hpp>
#include <algorithm>

namespace Numeric
{

DECLARE_ALGORITHM(MakeQuadratureScheme);

//======================================================================================================
/// Constructor. Declare algorithm properties.
MakeQuadratureScheme::MakeQuadratureScheme()
{
  // number of quadrature points
  declareInt("N",24);
  // integration interval
  declareString("Interval","-1.0,1.0");
  // FunctionFcatory init string for a weight function
  declareClass("Function",&FunctionFactory::instance());
  // TableWorkspace with the created qudrature data
  declareWorkspace("Quadrature");
  // guess start of the interval containing all integration points
  declareDouble("StartX",-1);
  // guess end of the interval containing all integration points
  declareDouble("EndX",1);
  declareWorkspace("Test");
  declareWorkspace("Basis");
}

//======================================================================================================
// Private namespace declarations
namespace 
{
  /// function x
  //double xfun(double x){return x;}

  class Xfun: public IFunction1D, public ParamFunction
  {
    size_t m_i;
  public:
    Xfun():m_i(0){}
    /// Returns the function's name
    virtual std::string name()const {return "Xfun";}
    /// Function you want to fit to.
    virtual void function1D(double* out, const double* xValues, const size_t nData)const
    {
      for(size_t i = 0; i < nData; ++i)
      {
        const double x = xValues[i];
        out[i] = x;// - (0.01 * m_i) *x*x;
      }
    }
    Xfun& operator()(size_t i){m_i = i; return *this;}
  };

  /// add function values to a table
  void addValuesToWorkspace(API::TableWorkspace_ptr tws, 
    const std::string& xColumn,
    const std::string& yColumn,
    const IFunction& fun,
    API::NumericColumn::PlotRole role = API::NumericColumn::Y);

  /// valuate a polynom recursively
  double evalPoly(size_t n, double x, const std::vector<double>& a, const std::vector<double>& b, const std::vector<double>& c);

  /// valuate a polynom recursively
  class EvalPoly: public IFunction1D, public ParamFunction
  {
    const size_t m_n;
    const std::vector<double>& m_a;
    const std::vector<double>& m_b;
    const std::vector<double>& m_c;
  public:
    EvalPoly(size_t n, const std::vector<double>& a, const std::vector<double>& b, const std::vector<double>& c):
    m_n(n),m_a(a),m_b(b),m_c(c)
    {}
    /// Returns the function's name
    virtual std::string name()const {return "EvalPoly";}
    /// Function you want to fit to.
    virtual void function1D(double* out, const double* xValues, const size_t nData)const
    {
      for(size_t i = 0; i < nData; ++i)
      {
        out[i] = evalPoly(m_n, xValues[i], m_a, m_b, m_c);
      }
    }
  };

  /// Check functions' orthonormality
  void checkOrtho(const std::vector<ChebFunction_sptr>& poly);

  /// Calculate the quadrature weights using the formula from Numerical Recipes
  void computeWeights(const std::vector<ChebFunction_sptr>& poly, const std::vector<double>& r, const ChebFunction& weight, std::vector<double>& w);

  /// Calculate the quadrature weights solving a set of linear equations
  void computeWeights1(const std::vector<ChebFunction_sptr>& poly, const std::vector<double>& r, const ChebFunction& weight, std::vector<double>& w);

  /// Compute the dvr basis associated with the quadrature
  void computeDVRBasis(
    API::TableWorkspace_ptr tws, 
    const std::vector<ChebFunction_sptr>& poly, 
    const ChebFunction& weight, 
    std::vector<double>& r,
    std::vector<double>& w
    );
}

//======================================================================================================
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

  int n = get("N");// it is the order of the polynomial
  n += 1; // turn it into the size of the problem
  if (n < 2)
  {
    throw std::invalid_argument("N must be > 1");
  }
  //std::string funStr = get("Function");
  //IFunction_sptr fun = FunctionFactory::instance().createFitFunction(funStr);
  IFunction_sptr fun = getClass("Function");

  weight.bestFit( *fun );

  for(size_t i = 0; i < weight.nfuns(); ++i)
  {
    const ScaledChebfun& f = const_cast<const ChebFunction&>(weight).fun(i);
    std::cerr << i << ' ' << f.n() << std::endl;
  }
  std::cerr << "Weight function integral " << weight.integr() << std::endl;
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

  tws->addColumn("double","k");
  // a-coefficients in the recurrence relation p_j+1 = (x-a_j)*p_j - b_j*p_j-1
  tws->addColumn("double","a");
  // b-coefficients in the recurrence relation p_j+1 = (x-a_j)*p_j - b_j*p_j-1
  tws->addColumn("double","b");
  tws->addColumn("double","c");
  tws->setRowCount(n);
  auto& a = tws->getDoubleData("a");
  auto& b = tws->getDoubleData("b");
  auto& c = tws->getDoubleData("c");

  startX = get("StartX");
  endX = get("EndX");
  {
    FunctionDomain1DVector xdomain(0, n-1, n);
    // !! it's not very good because it potentially can change the size of the column
    tws->getDoubleData("k").assign(xdomain.getPointerAt(0),xdomain.getPointerAt(0) + n);
  }

  auto test = API::TableWorkspace_ptr( new API::TableWorkspace );
  test->addColumn("double","x");
  test->getColumn("x")->asNumeric()->setPlotRole(API::NumericColumn::X);
  const size_t testSize = 1000;
  {
    //FunctionDomain1D_sptr xdomain( new FunctionDomain1DVector (startX, endX, testSize) );
    auto xdomain = weight.createDomainFromXPoints();
    test->setRowCount(xdomain->size());
    // !! it's not very good because it potentially can change the size of the column
    test->getDoubleData("x").assign(xdomain->getPointerAt(0),xdomain->getPointerAt(0) + xdomain->size());
  }
  setProperty("Test",test);

  std::vector<ChebFunction_sptr> poly( n );
  for(size_t i = 0; i < n; ++i)
  {
    poly[i] = ChebFunction_sptr( new ChebFunction(weight, false) );
  }

  std::vector<double> norms( n );

  *poly[0] = weight;

  // define the "x"-function
  Xfun xfun;

  // don't copy just use the same base
  ChebFunction pp(*poly[0]);
  pp *= *poly[0];
  norms[0] = pp.integr();
  pp *= xfun(0);
  a[0] = pp.integr() / norms[0];
  b[0] = 0.0;

  *poly[1] = *poly[0];
  *poly[1] *= xfun(1);
  pp = *poly[0];
  pp *= a[0];
  *poly[1] -= pp;

  pp = *poly[1];
  pp *= *poly[0];
  std::cerr << "integr " << pp.integr() << std::endl;

  for(size_t i = 2; i < n; ++i)
  {
    //std::cerr << "making " << i << "-th poly" << std::endl;
    size_t i1 = i - 1;
    size_t i2 = i - 2;
    // calculate a and b for this iteration
    pp = *poly[i1];
    pp *= *poly[i1];
    norms[i1] = pp.integr();
    pp *= xfun(i);
    a[i1] = pp.integr() / norms[i1];
    b[i1] = norms[i1] / norms[i2];

    //calculate i-th poly
    *poly[i] = *poly[i1];
    *poly[i] *= xfun(i);
    pp = *poly[i1];
    pp *= a[i1];
    *poly[i] -= pp;
    pp = *poly[i2];
    pp *= b[i1];
    *poly[i] -= pp;
  }
  pp = *poly.back();
  pp *= *poly.back();
  norms.back() = pp.integr();

  for(size_t i = 0; i < n; ++i)
  {
    const double tmp = 1.0 / sqrt( norms[i] );
    *poly[i] *= tmp;
    if ( i > 0 )
    {
      c[i-1] = sqrt( norms[i-1] / norms[i]  );
      a[i-1] *= c[i-1];
      b[i-1] *= c[i-1];
    }
    addValuesToWorkspace( test, "x", "p"+boost::lexical_cast<std::string>(i), *poly[i] );
    //pp = *poly[i];
    //pp *= *poly[i];
    //std::cerr << "<" << i << ">= " << pp.integr() << std::endl;
  }
  tws->removeRow( n-1 );

  //pp.fromDerivative( *poly.front() );
  //ChebFunction der2;
  //der2.fromDerivative( pp );
  //der2 /= *poly.front();
  //addValuesToWorkspace( test, "x", "deriv", der2 );

  auto basisWS = ChebfunVector_sptr( new ChebfunVector );
  for(size_t i = 0; i < n; ++i)
  {
    basisWS->add( poly[i] );
  }
  setProperty( "Basis", basisWS );

  // find the domain where the resultant polynomial isn't practically zero
  {
    auto xdomain = weight.createDomainFromXPoints();
    FunctionValues values( *xdomain );
    poly.back()->function( *xdomain, values );
    size_t i = 0;
    while( fabs(values.getCalculated(i)) < 1e-3 ) ++i;
    startX = (*xdomain)[i];
    i = values.size() - 1;
    while( i > 0 && fabs(values.getCalculated(i)) < 1e-3 ) --i;
    endX = (*xdomain)[i];
  }

  std::cerr << "Calculating abscissas on " << startX << " - " << endX << std::endl;
  ChebFunction res(0, startX, endX);
  //res.fit( *poly.back() );
  res.bestFit( *poly.back() );
  addValuesToWorkspace( test, "x", "res", res );
  std::cerr << "res size " << res.cfun(0).n() << std::endl;

  //if ( res.cfun(0).n() < 1000 )
  {
    std::vector<double> r;
    res.roots( r );
    std::cerr << "size " << r.size() << std::endl;

    // output the integration points and weights
    if ( r.size() == n - 1 )
    {
      std::sort( r.begin(), r.end() );

      tws->addColumn("double","r");
      assert(r.size() == tws->rowCount());
      tws->getDoubleData("r") = r;

      std::vector<double> w;
      computeWeights1( poly, r, weight, w );
      tws->addColumn("double","w");
      assert(w.size() == tws->rowCount());
      tws->getDoubleData("w") = w;

      double test = 0.0;
      for(size_t i = 0; i < r.size(); ++i)
      {
        test += w[i];
      }
      std::cerr << "test = " << test << std::endl;

      // output the "x"-function
      {
        FunctionDomain1DView xdomain( r );
        FunctionValues values( xdomain );
        xfun.function( xdomain, values );
        tws->addColumn("double","x");
        auto& xcol = tws->getDoubleData("x");
        for(size_t i = 0; i < xcol.size(); ++i)
        {
          xcol[i] = values[i];
        }
      }

      computeDVRBasis( tws, poly, weight, r, w );
    }
    else
    {
      std::cerr << "Failed to calculate the roots." << std::endl;
    }
  }

  checkOrtho(poly);
}

//======================================================================================================
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

//======================================================================================================
//    Private namespace definitions
//======================================================================================================
namespace 
{
//======================================================================================================
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
    const IFunction& fun,
    API::NumericColumn::PlotRole role
    )
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
    tws->getColumn( yColumn )->asNumeric()->setPlotRole(role);
    auto& y = tws->getDoubleData( yColumn );

    FunctionDomain1DView domain( x.data(), x.size() );
    FunctionValues values( domain );
    fun.function( domain, values );
    for(size_t i = 0; i < y.size(); ++i)
    {
      y[i] = values.getCalculated(i);
    }
  }

//======================================================================================================
  /**
   * Evaluate a polynom recursively: 
   * @param n :: Polynom order
   * @param x :: Abscissa at wich to evaluate the polynom.
   * @param a :: The a-coefficients of the recurrence.
   * @param b :: The b-coefficients of the recurrence.
   */
  double evalPoly(size_t n, double x, const std::vector<double>& a, const std::vector<double>& b, const std::vector<double>& c)
  {
    double p0 = 1.0;
    if ( n == 0 ) return p0;
    double p1 = ( c[0] * x - a[0] ) * p0;
    if ( n == 1 ) return p1;
    for(size_t i = 2; i < n; ++i)
    {
      double p = ( c[i-1] * x - a[i-1] ) * p1 - b[i-1] * p0;
      p0 = p1;
      p1 = p;
    }
    return p1;
  }

//======================================================================================================
  void checkOrtho(const std::vector<ChebFunction_sptr>& poly)
  {
    double norm = 0.0;
    double offd = 0.0;
    const size_t n = poly.size();
    for(size_t i = 0; i < n; ++i)
    {
      for(size_t j = i; j < n; ++j)
      { 
        ChebFunction pp( *poly[i] );
        pp *= *poly[j];
        const double d = fabs( pp.integr() );
        //std::cerr << "<" << i << "|" << j << ">=" << d << std::endl;
        if ( i == j ) norm += d;
        else
        {
          offd += d;
        }
      }
    }
    std::cerr << "orthonormality " << norm / n << ' ' << 2*offd / (n*(n-1)) << std::endl;
  }

//======================================================================================================
  /**
   * Compute the quadrature weights using w = 1/ (p[n-2] * p'[n-1]), where p'[n-1] is the 
   *  derivative of the last polynomial.
   * @param poly :: Vector of p*weight functions
   * @param r :: Quadrature abscissas (roots of the last polynomial)
   * @param weight :: Square root of the weight function
   * @param w :: Result weights
   */
  void computeWeights(const std::vector<ChebFunction_sptr>& poly, const std::vector<double>& r, const ChebFunction& weight, std::vector<double>& w)
  {
    size_t n = poly.size() - 1;
    w.resize( n );
    // compute the weights
    FunctionDomain1DView domain( r.data(), r.size() );
    // p_n-1
    FunctionValues pn1( domain ); 
    poly[n-1]->function( domain, pn1 );
    // p'_n
    FunctionValues dn( domain );
    ChebFunction der;
    der.fromDerivative(*poly[n]);
    der.function( domain, dn );
    FunctionValues pn( domain ); 
    poly[n]->function( domain, pn );
    // weight function
    FunctionValues wgt( domain );
    weight.function( domain, wgt );
    ChebFunction dweight;
    dweight.fromDerivative( weight );
    FunctionValues dwgt( domain );
    dweight.function( domain, dwgt );

    ChebFunction weight2(weight);
    weight2.square();
    double factor = 1.0 / weight2.integr();
    factor *= factor; // it is a wrong factor !!!

    for(size_t i = 0; i < n; ++i)
    {
      // w[i] = 1 / ( pn1*( dn/wgt^2 - pn*dwgt/wgt^3 ) )
      double tmp = pn1[i] * (dn[i] - pn[i]*dwgt[i]/wgt[i]) / (wgt[i] * wgt[i]);
      w[i] = factor / tmp;
    }

  }

//======================================================================================================
  /// This one works better than the other one.
  void computeWeights1(const std::vector<ChebFunction_sptr>& poly, const std::vector<double>& r, const ChebFunction& weight, std::vector<double>& w)
  {
    size_t n = poly.size() - 1;
    w.resize( n );
    GSLMatrix M(n,n);
    GSLVector rhs(n);
    rhs.zero();
    GSLVector x(n);

    ChebFunction weight2(weight);
    weight2.square();
    rhs.set(0, weight2.integr());
    std::cerr << "weight integr " << rhs[0] << std::endl;

    FunctionDomain1DView domain( r.data(), r.size() );
    FunctionValues wgt( domain );
    weight.function( domain, wgt );
    FunctionValues values( domain );

    for(size_t i = 0; i < n; ++i)
    {
      if ( i == 0 )
      {
        for(size_t j=0; j < n; ++j)
        {
          M.set( i, j, 1.0);
        }
      }
      else
      {
        poly[i]->function( domain, values );
        for(size_t j=0; j < n; ++j)
        {
          M.set( i, j, values[j] / wgt[j] );
        }
      }
    }
    //std::cerr << M << std::endl;
    M.solve(rhs,x);
    for(size_t i = 0; i < n; ++i)
    {
      w[i] = x[i];
    }
  }

//======================================================================================================
  /**
   * Compute the dvr basis associated with the quadrature.
   * @param tws    :: Table workspace to store the basis.
   * @param poly   :: The basis as chebfuns.
   * @param weight :: The square root of the quadrature weight function.
   * @param r      :: The quadrature ascissas.
   * @param w      :: The quadrature weights.
   */
  void computeDVRBasis(
    API::TableWorkspace_ptr tws, 
    const std::vector<ChebFunction_sptr>& poly, 
    const ChebFunction& weight, 
    std::vector<double>& r,
    std::vector<double>& w
    )
  {
    // the basis size
    const size_t n = r.size();

    // derivative of the weight function
    ChebFunction dweight;
    dweight.fromDerivative( weight );
    FunctionDomain1DView domain( r );
    FunctionValues wgt( domain );
    FunctionValues dwgt( domain );
    weight.function( domain, wgt );
    dweight.function( domain, dwgt );

    // take a square root of the quadrature weights
    std::vector<double> wr( n );
    for(size_t i = 0; i < n; ++i)
    {
      wr[i] = sqrt( w[i] );
    }

    // output function values, first and second derivatives
    for(size_t i = 0; i < n; ++i)
    {
      const std::string si = boost::lexical_cast<std::string>(i);
      // compute and save the ith basis function
      FunctionValues p( domain );
      poly[i]->function( domain, p );

      tws->addDoubleColumn("p"+si);
      auto& fi = tws->getDoubleData("p"+si);

      for(size_t k = 0; k < n; ++k)
      {
        fi[k] = wr[k] / wgt[k] * p[k];
      }

      // compute and save the derivative of the ith basis function
      ChebFunction dpoly;
      dpoly.fromDerivative( *poly[i] );
      FunctionValues dp( domain );
      dpoly.function( domain, dp );

      tws->addDoubleColumn("d"+si);
      auto& di = tws->getDoubleData("d"+si);

      for(size_t k = 0; k < n; ++k)
      {
        di[k] = wr[k] / wgt[k] * ( dp[k] );
      }

    }
  }

} // namespace
//======================================================================================================


} // namespace Numeric
