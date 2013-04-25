#include "Numeric/NonPolynomial.h"
#include "Numeric/ConstantFunction.h"

namespace Numeric
{

/**
 * Constructor.
 */
NonPolynomial::NonPolynomial():
    Polynomial(),
    m_startX(0.0),
    m_endX(1.0)
{
}

/**
  Creates a new instance of the generic polynomial.

  @param n :: Polynomial order.
  @param xfun :: Function that plays role of \f$x\f$.
  */
NonPolynomial::NonPolynomial(int n, IFunction_sptr xfun):
    Polynomial(n),
    m_xfun(xfun),
    m_startX(0.0),
    m_endX(1.0)
{
    if ( !boost::dynamic_pointer_cast<IFunction1D>(xfun) )
    {
        throw std::runtime_error("X-function can only be an instance of IFunction1D in NonPolynomial.");
    }
}

/**
    Non-polynomials will be weightless for now.
  */
double NonPolynomial::weightIntegral() const
{
    return 1.0;
}

/**
  Replaces the x-function for this generic polynomial.

  @param xfun :: Function that plays role of \f$x\f$.
  */
void NonPolynomial::setXFunction(IFunction_sptr xfun)
{
    if ( !boost::dynamic_pointer_cast<IFunction1D>(xfun) )
    {
        throw std::runtime_error("X-function can only be an instance of IFunction1D in NonPolynomial.");
    }
    m_xfun = xfun;
}

/**
  Calculate coefficients for the recurrence relationships.
  */
void NonPolynomial::updateABC() const
{
    m_a.resize(m_n);
    m_b.resize(m_n);
    m_c.resize(m_n);

//    chebfun sqrtWgt(m_fun);
//    try
//    {
//      sqrtWgt.sqrt();
//    }
//    catch(...)
//    {
//      throw std::runtime_error("Weight function must be positive everywhere");
//    }

//    if ( sqrtWgt.n() < m_n )
//    {
//      //sqrtWgt.resize(m_n + sqrtWgt.n());
//      sqrtWgt.resize( 2*m_n );
//    }

//    const size_t nn = m_n + 1;
//    std::vector<chebfun_sptr> poly( nn );
//    for(size_t i = 0; i < nn; ++i)
//    {
//      poly[i] = chebfun_sptr( new chebfun(sqrtWgt) );
//    }

//    std::vector<double> norms( nn );

//    *poly[0] = sqrtWgt;

//    // define the "x"-function
//    Xfun xfun;

//    chebfun pp(*poly[0]);
//    pp *= *poly[0];
//    norms[0] = pp.integr();
//    pp *= xfun(0);
//    m_a[0] = pp.integr() / norms[0];
//    m_b[0] = 0.0;

//    *poly[1] = *poly[0];
//    *poly[1] *= xfun(1);
//    pp = *poly[0];
//    pp *= m_a[0];
//    *poly[1] -= pp;

//    pp = *poly[1];
//    pp *= *poly[0];
//    //std::cerr << "integr " << pp.integr() << std::endl;

//    for(size_t i = 2; i < nn; ++i)
//    {
//      //std::cerr << "making " << i << "-th poly" << std::endl;
//      size_t i1 = i - 1;
//      size_t i2 = i - 2;
//      // calculate a and b for this iteration
//      pp = *poly[i1];
//      pp *= *poly[i1];
//      norms[i1] = pp.integr();
//      pp *= xfun(i);
//      m_a[i1] = pp.integr() / norms[i1];
//      m_b[i1] = norms[i1] / norms[i2];

//      //calculate i-th poly
//      *poly[i] = *poly[i1];
//      *poly[i] *= xfun(i);
//      pp = *poly[i1];
//      pp *= m_a[i1];
//      *poly[i] -= pp;
//      pp = *poly[i2];
//      pp *= m_b[i1];
//      *poly[i] -= pp;
//    }
//    pp = *poly.back();
//    pp *= *poly.back();
//    norms.back() = pp.integr();

//    for(size_t i = 0; i < nn; ++i)
//    {
//      const double tmp = 1.0 / sqrt( norms[i] );
//      *poly[i] *= tmp;
//      if ( i > 0 )
//      {
//        m_c[i-1] = sqrt( norms[i-1] / norms[i]  );
//        m_a[i-1] *= m_c[i-1];
//        m_b[i-1] *= m_c[i-1];
//      }
//    }
}

IFunction_const_sptr NonPolynomial::createWeightFunction() const
{
    ConstantFunction *fun = new ConstantFunction();
    fun->setParameter(0, 1.0);
    return IFunction_const_sptr(fun);
}

IFunction_const_sptr NonPolynomial::createWeightDerivative() const
{
    ConstantFunction *fun = new ConstantFunction();
    fun->setParameter(0, 0.0);
    return IFunction_const_sptr(fun);
}

} // Numeric
