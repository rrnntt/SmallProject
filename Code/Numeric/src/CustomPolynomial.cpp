#include "Numeric/CustomPolynomial.h"
#include "Numeric/ChebFunction.h"
#include "Numeric/FunctionFactory.h"

namespace Numeric
{

DECLARE_FUNCTION(CustomPolynomial)

// Private namespace declarations
namespace 
{
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

  DECLARE_FUNCTION(Xfun)

  class XExp2: public IFunction1D, public ParamFunction
  {
  public:
    XExp2()
    {
      declareParameter("Sigma",1.0,"Defins thewidth of this functions.");
    }
    /// Returns the function's name
    virtual std::string name()const {return "XExp2";}
    /// Function you want to fit to.
    virtual void function1D(double* out, const double* xValues, const size_t nData)const
    {
      const double sigma = getParameter(0);
      for(size_t i = 0; i < nData; ++i)
      {
        const double x = xValues[i];
        out[i] = x * exp( - x*x / 2 * sigma );
      }
    }
  };

  DECLARE_FUNCTION(XExp2)
}

/**
 * Constructor.
 */
CustomPolynomial::CustomPolynomial():Polynomial(),m_xfun(new Xfun)
{
}

/**
 * Constructor.
 */
CustomPolynomial::CustomPolynomial(int n, const double& startX,const double& endX):
Polynomial(n),
m_fun(n,startX,endX),
m_xfun(new Xfun)
{
}

/**
 * Destructor.
 */
CustomPolynomial::~CustomPolynomial()
{
}

/// Set the weight function
void CustomPolynomial::setWeightFunction(IFunction_const_sptr wgtFun, const double& tol)
{
  m_weightFunction = wgtFun;
  auto cheb = dynamic_cast<const ChebFunction*>( wgtFun.get() );
  if ( cheb && cheb->nfuns() == 1 && !cheb->cfun(0).hasScaling() )
  {
      m_fun = cheb->cfun(0).getUnscaled();
  }
  else
  {
    m_fun.bestFit( *wgtFun, tol );
  }
}

/**
  Define the function which will be used instead of x in polynomial expression.
  */
void CustomPolynomial::setXFunction(IFunction_sptr fun)
{
  throw std::runtime_error("To use non-linear x's inverse function must be implemented.");
  m_xfun = fun;
}

/// Recalculate (re-fill) m_a, m_b, m_c
void CustomPolynomial::updateABC() const
{
  m_a.resize(m_n);
  m_b.resize(m_n);
  m_c.resize(m_n);

  chebfun sqrtWgt(m_fun);
  try
  {
    sqrtWgt.sqrt();
  }
  catch(...)
  {
    throw std::runtime_error("Weight function must be positive everywhere");
  }

  if ( sqrtWgt.n() < m_n )
  {
    //sqrtWgt.resize(m_n + sqrtWgt.n());
    sqrtWgt.resize( 10*m_n );
  }

  //std::cerr << "n = " << sqrtWgt.n() << ' ' << m_fun.integr() << std::endl;

  const size_t nn = m_n + 1;
  std::vector<chebfun_sptr> poly( nn );
  for(size_t i = 0; i < nn; ++i)
  {
    poly[i] = chebfun_sptr( new chebfun(sqrtWgt) );
  }

  std::vector<double> norms( nn );

  *poly[0] = sqrtWgt;

  // define the "x"-function
  if ( ! m_xfun ) 
  {
    throw std::runtime_error("X-function is undefined in CustomPolynomial.");
  }
  IFunction &xfun = *m_xfun;

  chebfun pp(*poly[0]);
  pp *= *poly[0];
  norms[0] = pp.integr();
  if (norms[0] == 0.0)
  {
    throw std::runtime_error("Custom polynomial: zero integral.");
  }
  pp *= xfun;
  m_a[0] = pp.integr() / norms[0];
  m_b[0] = 0.0;

  *poly[1] = *poly[0];
  *poly[1] *= xfun;
  pp = *poly[0];
  pp *= m_a[0];
  *poly[1] -= pp;

  pp = *poly[1];
  pp *= *poly[0];
  //std::cerr << "integr " << pp.integr() << std::endl;

  for(size_t i = 2; i < nn; ++i)
  {
    //std::cerr << "making " << i << "-th poly" << std::endl;
    size_t i1 = i - 1;
    size_t i2 = i - 2;
    // calculate a and b for this iteration
    pp = *poly[i1];
    pp *= *poly[i1];
    norms[i1] = pp.integr();
    pp *= xfun;
    m_a[i1] = pp.integr() / norms[i1];
    m_b[i1] = norms[i1] / norms[i2];
    m_c[i1] = 1.0;

    //calculate i-th poly
    *poly[i] = *poly[i1];
    *poly[i] *= xfun;
    pp = *poly[i1];
    pp *= m_a[i1];
    *poly[i] -= pp;
    pp = *poly[i2];
    pp *= m_b[i1];
    *poly[i] -= pp;
  }
  pp = *poly.back();
  pp *= *poly.back();
  norms.back() = pp.integr();

  for(size_t i = 0; i < nn-1; ++i)
  {
      m_c[i] = sqrt( norms[i] / norms[i+1] );
      m_a[i] *= m_c[i];
//    const double tmp = 1.0 / sqrt( norms[i] );
//    *poly[i] *= tmp;
    if ( i > 0 )
    {
      m_b[i] *= m_c[i] * m_c[i-1];
    }
    //std::cerr << i << ' ' << m_a[i] << ' ' << m_b[i] << ' ' << m_c[i] << std::endl;
  }
//  for(size_t i = 0; i < poly.size(); ++i)
//  {
//      std::cerr << "n(" << i <<")=" << poly[i]->n() << ' ' << poly[i]->startX() << ' ' << poly[i]->endX()  << std::endl;
//      for(size_t j = 0; j <= i; ++j)
//      {
//          chebfun t = *poly[i];
//          t *= *poly[j];
//          std::cerr << i << ',' << j << ' ' << t.integr() << std::endl;
//      }
//  }
}

/// Returns the unscaled integral of the weight function
double CustomPolynomial::weightIntegral() const
{
    return m_fun.integr();
}

/**
Override function evaluation to use m_xfun instead of x.
*/
void CustomPolynomial::function1D(double *out, const double *xValues, const size_t nData) const
{
    std::vector<double> x(nData);
    IFunction1D_sptr xfun = boost::dynamic_pointer_cast<IFunction1D>( m_xfun );
    if ( !xfun )
    {
      throw std::runtime_error("X-function in CustomPolynomial must be 1D");
    }
    xfun->function1D(&x[0],xValues,nData);
//    for(size_t i = 0; i < nData; ++i)
//    {
//        std::cerr << "x("<<i<<")="<<x[i] << std::endl;
//    }
    Polynomial::function1D(out,&x[0],nData);
}

/// Throws exception if weight function isn't set.
IFunction_const_sptr CustomPolynomial::createWeightFunction() const
{
  if ( !m_weightFunction ) 
  {
    throw std::runtime_error("Weight function hasn't been set for CustomPolynomial.");
  }
  return m_weightFunction;
}

IFunction_const_sptr CustomPolynomial::createWeightDerivative() const
{
  if ( !m_weightFunction ) 
  {
    throw std::runtime_error("Weight function hasn't been set for CustomPolynomial.");
  }
  auto deriv = ChebFunction::create();
  deriv->fromDerivative( ChebFunction(m_fun) );
  return deriv;
}

} // Numeric
