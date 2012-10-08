#include "Numeric/CustomPolynomial.h"

namespace Numeric
{

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
}

/**
 * Constructor.
 */
CustomPolynomial::CustomPolynomial():Polynomial()
{
}

/**
 * Constructor.
 */
CustomPolynomial::CustomPolynomial(int n, const double& startX,const double& endX):
Polynomial(n),
m_fun(n,startX,endX)
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
  m_fun.bestFit( *wgtFun, tol );
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
    sqrtWgt.resize( 2*m_n );
  }

  const size_t nn = m_n + 1;
  std::vector<chebfun_sptr> poly( nn );
  for(size_t i = 0; i < nn; ++i)
  {
    poly[i] = chebfun_sptr( new chebfun(sqrtWgt) );
  }

  std::vector<double> norms( nn );

  *poly[0] = sqrtWgt;

  // define the "x"-function
  Xfun xfun;

  chebfun pp(*poly[0]);
  pp *= *poly[0];
  norms[0] = pp.integr();
  pp *= xfun(0);
  m_a[0] = pp.integr() / norms[0];
  m_b[0] = 0.0;

  *poly[1] = *poly[0];
  *poly[1] *= xfun(1);
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
    pp *= xfun(i);
    m_a[i1] = pp.integr() / norms[i1];
    m_b[i1] = norms[i1] / norms[i2];

    //calculate i-th poly
    *poly[i] = *poly[i1];
    *poly[i] *= xfun(i);
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

  for(size_t i = 0; i < nn; ++i)
  {
    const double tmp = 1.0 / sqrt( norms[i] );
    *poly[i] *= tmp;
    if ( i > 0 )
    {
      m_c[i-1] = sqrt( norms[i-1] / norms[i]  );
      m_a[i-1] *= m_c[i-1];
      m_b[i-1] *= m_c[i-1];
    }
  }
}

/// Returns the unscaled integral of the weight function
double CustomPolynomial::weightIntegral() const
{
  return m_fun.integr();
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

} // Numeric
