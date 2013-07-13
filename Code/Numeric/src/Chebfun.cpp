#include "Numeric/Chebfun.h"
#include "Numeric/IFunction.h"
#include "Numeric/FunctionDomain1D.h"
#include "Numeric/FunctionValues.h"

#include <gsl/gsl_errno.h>
#include <gsl/gsl_fft_real.h>
//#include <gsl/gsl_integration.h>
#include <gsl/gsl_fft_halfcomplex.h>
#include <gsl/gsl_eigen.h>

#include <boost/lexical_cast.hpp>

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <iostream>
#include <limits>

namespace Numeric
{
  /**
   * Calculate the m_x array as zeroes of the m_n-th order chebyshev polynomial.
   * If the array is shared x values will change for all chebfuns
   */
  void ChebfunBase::calcX()
  {
    x.resize(n+1);
    w.resize(n+1);
    iw.resize(n+1);
    iw2.resize(n+1);
    double x0 = (startX + endX)/2;
    double b  = (endX - startX)/2;
    const double pin = M_PI/n;
    for(size_t i = 0; i < x.size(); ++i)
    {
      size_t j = n - i;
      const double pi_i_n = pin*i;
      const double c = cos( pi_i_n );
      x[j] = x0 + b * c;
      w[j] = 1.;
      if ( j % 2 != 0 ) w[j] = -1.;
      if ( j == 0 || j == n ) w[j] /= 2;
      //std::cerr<<"x["<<j<<"]="<<x[j]<<' ' << '\n';
      if ( i % 2 == 0)
      {
        iw[i] = 2.0 / ( 1.0 - double( i * i ) );
      }
      else
      {
        iw[i] = 0.0;
      }
      if ( i == 0 || j == 0 )
      {
        iw2[j] = 0.0;
      }
      else
      {
        const double s = sin( pi_i_n );
        iw2[j] = pin * s * s;
      }
    }
  }

  chebfun::chebfun(size_t n, const double& startX,const double& endX)
  {
    if (n >= 0)
    {
      set(n,startX,endX);
    }
  }

  /**
    * Create chebfun with x array shared with other
    */
  chebfun::chebfun(const chebfun& other):
  m_base(other.m_base)
  {
    m_p.assign(other.m_p.begin(),other.m_p.end());
    m_a.assign(other.m_a.begin(),other.m_a.end());
  }

  /**
    * Create a copy of the other. x array is shared.
    */
  chebfun& chebfun::operator=(const chebfun& other)
  {
    m_base = other.m_base;
    m_p.assign(other.m_p.begin(),other.m_p.end());
    m_a.assign(other.m_a.begin(),other.m_a.end());
    return *this;
  }

  /**
    * Resets the x array (if it isn't shared)
    */
  void chebfun::set(size_t n,const double& startX,const double& endX)
  {
    m_base.reset( new ChebfunBase(n, startX, endX) );
    m_p.resize( n + 1 );
    invalidateA();
  }

  /**
   * Set the y points from a vector
   * @param y :: Vector with y- points. y.size() must be eq n() + 1
   */
  void chebfun::setP(const std::vector<double>& y)
  {
    const size_t n = y.size();
    if (m_p.size() != n) throw std::runtime_error("chebfun::setP: Wrong size " + 
      boost::lexical_cast<std::string>(m_p.size()) + " " + boost::lexical_cast<std::string>(n)
      );
    m_p.assign( y.begin(), y.end() );
    invalidateA();
  }


  /**
   * Set the y points from a GSL vector
   * @param y :: Vector with y- points. y.size() must be eq n() + 1
   */
  void chebfun::setP(const GSLVector& y)
  {
    if ( y.size() != n() + 1 ) throw std::runtime_error("Cannot set y-points: different sizes.");
    for(size_t i = 0; i < y.size(); ++i)
    {
      m_p[i] = y[i];
    }
    invalidateA();
  }

  /**
   * Set the y points from a column of a GSL matrix
   * @param M :: Matrix with columns containing y-points.
   * @param col :: Column index to copy from.
   */
  void chebfun::setP(const GSLMatrix& M, size_t col)
  {
    const size_t N = M.size2();
    if ( N != n() + 1 ) throw std::runtime_error("Cannot set y-points: different sizes.");
    for(size_t i = 0; i < N; ++i)
    {
      m_p[i] = M.get( i, col );
    }
    invalidateA();
  }

  /**
    * Calculate the function value as a T form expansion at point x
    * @param x :: Argument of the function
    */
  double chebfun::valueT(const double& x)const
  {
    if ( m_a.empty() )
    {
      calcA();
    }
    double c = 2./( endX() - startX() );
    double a = 1. - c * endX();
    // scale to interval -1 < x < 1
    double xx = a + c * x;
    double b2 = 0.;
    double b1 = 0.;
    double b;
    for(size_t j = n(); j > 0; --j)
    {
      b = b1*xx*2 - b2 + m_a[j];
      b2 = b1;
      b1 = b;
    }
    return xx * b1 - b2 +  m_a[0];
  }

  /**
    * Calculate the function value as a U form expansion at point x
    * @param x :: Argument of the function
    */
  double chebfun::valueU(const double& x)const
  {
    if ( m_a.empty() )
    {
      calcA();
    }
    double c = 2./(endX() - startX());
    double a = 1. - c * endX();
    // scale to interval -1 < x < 1
    double xx = a + c * x;
    double b2 = 0.;
    double b1 = 0.;
    double b;
    for(size_t j = n(); j > 0; --j)
    {
      b = b1*xx*2 - b2 + m_a[j];
      b2 = b1;
      b1 = b;
    }
    return 2. * xx * b1 - b2 +  m_a[0];
  }

  /**
    * Calculate the derivative of the T form expansion at point x
    * @param x :: Argument of the function
    */
  double chebfun::derivT(const double& x)const
  {
    if ( m_a.empty() )
    {
      calcA();
    }
    double c = 2./(endX() - startX());
    double a = 1. - c * endX();
    // scale to interval -1 < x < 1
    double xx = a + c * x;
    double b2 = 0.;
    double b1 = 0.;
    double b;
    for(size_t j = n() - 1; j > 0; --j)
    {
      b = b1*xx*2 - b2 + m_a[j + 1]*(j+1);
      b2 = b1;
      b1 = b;
    }
    return 2. * xx * b1 - b2 + m_a[1];
  }

  /**
    * Calculate the derivative of the U form expansion at point x
    * @param x :: Argument of the function
    */
  double chebfun::derivT2(const double& x)const
  {
    if ( m_a.empty() )
    {
      calcA();
    }
    size_t n1 = n();
    double n2 = double(n1)*n1;
    double res = n2*(n2-1)/3;
    if ( n1 % 2 != 0 ) res = -res;
    return res;

    //double c = 2./(m_endX - m_startX);
    //double a = 1. - c * m_endX;
    //// scale to interval -1 < x < 1
    //double xx = a + c * x;
    //double bT2 = 0.;
    //double bT1 = 0.;
    //double bT;
    //double bU2 = 0.;
    //double bU1 = 0.;
    //double bU;
    //double nn1 = m_n*(m_n+1);
    //for(int j = m_n; j > 0; --j)
    //{
    //  bT = bT1*xx*2 - bT2 + m_a[j];
    //  bT2 = bT1;
    //  bT1 = bT;
    //  bU = bU1*xx*2 - bU2 + m_a[j];
    //  bU2 = bU1;
    //  bU1 = bU;
    //}
    //return 2. * xx * b1 - b2 +  m_a[0];
  }

  /**
   * Calculate the value using teh barycenteric interpolation formula
   */
  double chebfun::valueB(const double& x)const
  {
    auto & X = m_base->x;
    auto & w = m_base->w;
    std::vector<double>::const_iterator it = std::find( X.begin(), X.end(), x );
    if (it != X.end())
    {
      std::vector<double>::difference_type i = std::distance(std::vector<double>::const_iterator(X.begin()),it);
      return m_p[i];
    }
    double res = 0;
    double weight = 0;
    for(size_t i=0;i<X.size();++i)
    {
      double W = w[i]/(x - X[i]);
      weight += W;
      res += W * m_p[i];
      //std::cerr << i << ' ' << (*m_w)[i] << ' ' << W << ' ' << W * m_p[i] << std::endl;
    }
    return res / weight;
  }

      /**
       * Class for helping to read the transformed data. It represent an output of the
       * GSL real fast fourier transform routine. The routine transforms an array of n
       * real numbers into an array of about n/2 complex numbers which are the amplitudes of
       * the positive frequencies of the full complex fourier transform. 
       */
      class HalfComplex
      {
        size_t m_size;           ///< size of the transformed data
        double* m_data; ///< pointer to the transformed data
        bool m_even;          ///< true if the size of the original data is even
      public:
        /**
         * Constructor. 
         * @param data A pointer to the transformed complex data
         * @param n The size of untransformed real data
         */
        HalfComplex(double* data,const size_t& n):m_size(n/2+1),m_data(data),m_even(n/2*2==n)
        {
        }
        /// Returns the size of the transform
        size_t size()const{return m_size;}
        /**
         * The real part of i-th transform coefficient
         * @param i The index of the complex transform coefficient
         * @return The real part 
         */
        double real(size_t i)const
        {
          if (i >= m_size) return 0.;
          if (i == 0) return m_data[0];
          return m_data[2*i-1];
        }
        /**
         * The imaginary part of i-th transform coefficient
         * @param i The index of the complex transform coefficient
         * @return The imaginary part 
         */
        double imag(size_t i)const
        {
          if (i >= m_size) return 0.;
          if (i == 0) return 0;
          if (m_even && i == m_size-1) return 0;
          return m_data[2*i];
        }
        /**
         * Set a new value for i-th complex coefficient
         * @param i The index of the coefficient
         * @param re The real part of the new value
         * @param im The imaginary part of the new value
         */
        void set(size_t i,const double& re,const double& im)
        {
          if (i >= m_size) return;
          if (i == 0)// this is purely real
          {
            m_data[0] = re;
          }
          else if (m_even && i == m_size-1)// this is also purely real
          {
            m_data[2*i-1] = re;
          }
          else
          {
            m_data[2*i-1] = re;
            m_data[2*i] = im;
          }
        }
      };

  /**
   * Calculate the chebyshev expansion coefficients from the points m_x
   */
  void chebfun::calcA() const
  {
    size_t m_n = n();
    size_t nn = m_n + 1;
    m_a.resize(nn);

    //// This is a correct and direct transform from m_p to m_a
    //// DO NOT DELETE !!!
    //for(int i = 0; i < nn; ++i)
    //{
    //  double t = 0.;
    //  for(int j = 0; j <= m_n; j++)
    //  {
    //    double p = m_p[m_n - j];
    //    if (j== 0 || j == m_n) p /= 2;
    //    t += cos(M_PI*i*(double(j))/m_n)*p;
    //  }
    //  m_a[i] = 2*t/m_n;
    //  //if (i == 0) m_a[0] /= 2;
    //}
    //m_a[0] /= 2;
    //m_a[m_n] /= 2;
    //return;
    //// End of the correct and direct transform from m_p to m_a

    if (m_n > 0)
    {
        // This is a magic trick which uses real fft to do the above cosine transform
        std::vector<double> tmp(m_n*2);
        std::reverse_copy(m_p.begin(),m_p.end(),tmp.begin());
        std::copy(m_p.begin()+1,m_p.end()-1,tmp.begin()+m_n+1);

        gsl_fft_real_workspace * workspace = gsl_fft_real_workspace_alloc(2*m_n);
        gsl_fft_real_wavetable * wavetable = gsl_fft_real_wavetable_alloc(2*m_n);
        gsl_fft_real_transform (&tmp[0], 1, 2*m_n, wavetable, workspace);
        gsl_fft_real_wavetable_free (wavetable);
        gsl_fft_real_workspace_free (workspace);

        HalfComplex fc(&tmp[0],tmp.size());
        for(size_t i=0; i < nn; ++i)
        {
          double a = fc.real(i)/m_n;
          //if (i == 0) a /= 2;
          m_a[i] = a;
          //std::cerr << fc.real(i) << ',' << fc.imag(i) << std::endl;
        }//*/
        m_a[0] /= 2;
        m_a[m_n] /= 2;
        // End of the magic trick
    }
    else
    {
        m_a[0] = m_p[0];
    }
  }

  void chebfun::calcP()
  {
    if ( m_a.empty() )
    {
      throw std::runtime_error("chebfun: cannot calculate P from A - A is empty.");
    }
    if ( n() + 1 != m_a.size() )
    {
      throw std::runtime_error("chebfun: cannot calculate P from A - different sizes: "+
        boost::lexical_cast<std::string>(n()+1)+" != " + boost::lexical_cast<std::string>(m_a.size()));
    }
    if ( m_p.size() != m_a.size() )
    {
      m_p.resize( m_a.size() );
    }
    //auto& x = m_base->x;
    //for(size_t i = 0; i < m_a.size(); ++i)
    //{
    //  m_p[i] = valueT( x[i] );
    //}
    size_t m_n = n();
    if ( m_n > 0 )
    {
        size_t nn = m_n + 1;
        std::vector<double> tmp(m_n*2);
        HalfComplex fc(&tmp[0],tmp.size());
        for(size_t i=0; i < nn; ++i)
        {
          double a = m_a[i] /2;// * m_n;
          if (i == 0) a *= 2;
          fc.set( i, a, 0.0 );
        }
        gsl_fft_real_workspace * workspace = gsl_fft_real_workspace_alloc(2*m_n);
        gsl_fft_halfcomplex_wavetable * wavetable = gsl_fft_halfcomplex_wavetable_alloc(2*m_n);

        gsl_fft_halfcomplex_transform (tmp.data(), 1, 2*m_n, wavetable, workspace);

        gsl_fft_halfcomplex_wavetable_free (wavetable);
        gsl_fft_real_workspace_free (workspace);

        std::reverse_copy( tmp.begin(), tmp.begin() + nn, m_p.begin() );
    }
    else
    {
        m_p[0] = m_a[0];
    }
  }

  /// make this chebfun a derivative of the argument
  void chebfun::fromDerivative(const chebfun& fun)
  {
    m_base = fun.m_base;
    //double dx = 1e-10*(endX() - startX());
    //std::vector<double> tmp(n() + 1);
    //m_p.resize(n() + 1);
    //auto& x = m_base->x;
    //for(size_t i = 0; i < m_p.size(); ++i)
    //{
    //  tmp[i] = (fun.valueB(x[i] + dx) - fun.m_p[i]) / dx;
    //}
    //std::copy(tmp.begin(), tmp.end(), m_p.begin());
    //invalidateA();
    m_a.resize( n() + 1 );
    auto& a = fun.coeffs();
    const size_t nn = n();
    m_a[nn] = 0.0;
    if ( nn > 0 )
    {
        m_a[nn - 1] = 2 * nn * a[nn];
        for(size_t k = nn - 1; k >= 2; --k)
        {
          m_a[k - 1] = m_a[k + 1] + 2 * k * a[k];
        }
        m_a[0] = m_a[2] / 2 + a[1];
        const double Dx = (endX() - startX())/2;
        for(size_t i = 0; i < m_a.size(); ++i)
        {
          m_a[i] /= Dx;
        }
    }
    calcP();
  }

  /// make this chebfun a second derivative of the argument
  void chebfun::fromDerivative2(const chebfun& fun)
  {
    m_base = fun.m_base;
    const size_t nn = n();
    m_a.resize( nn + 1 );
    auto& a = fun.coeffs();

    std::vector<double> b( nn + 1 );
    b[nn] = 0.0;
    b[nn - 1] = 2 * nn * a[nn];
    for(size_t k = nn - 1; k >= 2; --k)
    {
      b[k - 1] = b[k + 1] + 2 * k * a[k];
    }
    b[0] = b[2] / 2 + a[1];

    m_a[nn] = 0.0;
    m_a[nn - 1] = 2 * nn * b[nn];
    for(size_t k = nn - 1; k >= 2; --k)
    {
      m_a[k - 1] = m_a[k + 1] + 2 * k * b[k];
    }
    m_a[0] = m_a[2] / 2 + b[1];
    double Dx = (endX() - startX())/2;
    Dx *= Dx;
    for(size_t i = 0; i < m_a.size(); ++i)
    {
      m_a[i] /= Dx;
    }
    calcP();
  }

  double chebfun::integrate()
  {
    std::vector<double> &w = m_base->iw3;
    if ( w.empty() )
    {
      size_t N = n();
      w.resize( N + 1 );
      std::vector<double> &iw = m_base->iw;
      for(size_t i = 0; i < iw.size(); ++i)
      {
        double b = 0.0;
        for(size_t j = 0; j < iw.size(); ++j)
        {
          double t = iw[j];
          if ( j == 0 || j == N ) t /= 2;
          b += t * cos(M_PI*double(i*j)/N);
        }
        b /= N;
        if ( i != 0 && i != N ) b *= 2;
        w[i] = b;
        //std::cerr << i << ' ' << b << std::endl;
      }
    }
    double res = 0.0;
    auto &p = ypoints();
    for(size_t i = 0; i < w.size(); ++i)
    {
      res += w[i] * p[i];
    }
    return res * (endX() - startX()) / 2;
  }

  double chebfun::integr() const
  {
    double res = 0.0;
    const size_t n = m_base->n;
    auto& iw = m_base->iw;
    auto& a = coeffs();
    for(size_t i = 0; i < a.size(); i+=2)
    {
      res += a[i] * iw[i];
    }
    return res * (endX() - startX()) / 2;
  }

  double chebfun::integrateWeighted() const
  {
    double res = 0.0;
    const size_t n = m_base->n;
    auto& iw2 = m_base->iw2;
    auto& p = ypoints();
    auto& x = xpoints();
    for(size_t i = 1; i < p.size() - 1; ++i)
    {
      res += p[i] * iw2[i];
    }
    return res * (endX() - startX()) / 2;
  }

  double chebfun::norm2()
  {
    chebfun tmp;
    tmp = *this;
    tmp *= *this;
    return tmp.integr();
  }

  /**
   * Get norms of the base functions == sqrt(baseFunction integrals).
   *
   * @return :: Vector of norms.
   */
  std::vector<double> chebfun::baseNorm() const
  {
      chebfun c( *this );
      std::vector<double> p(n()+1,0.0);
      std::vector<double> norms(p);
      for(size_t i = 0; i < p.size(); ++i)
      {
          p[i] = 1.0;
          if ( i > 0 ) p[i - 1] = 0.0;
          c.setP( p );
          c.square();
          norms[i] = std::sqrt( c.integr() );
      }
      return norms;
  }

  void chebfun::fit(const IFunction& ifun)
  {
    auto& x = m_base->x;
    FunctionDomain1DView domain(x.data(),x.size());
    FunctionValues values(domain);
    ifun.function(domain,values);
    for(size_t i=0; i < x.size(); ++i)
    {
      m_p[i] = values.getCalculated(i);
    }
    invalidateA();
  }

  void chebfun::fit(AFunction f)
  {
    auto& x = m_base->x;
    for(size_t i = 0; i < x.size(); ++i)
    {
      m_p[i] = f( x[i] );
    }
    invalidateA();
  }

  chebfun& chebfun::operator+=(const chebfun& f)
  {
    if ( haveSameBase(f) )
    {
      for(size_t i = 0; i < m_p.size(); ++i)
      {
        m_p[i] += f.m_p[i];
      }
    }
    else
    {
      auto& x = m_base->x;
      for(size_t i = 0; i < m_p.size(); ++i)
      {
        m_p[i] += f( x[i] );
      }
    }
    invalidateA();
    return *this;
  }

  chebfun& chebfun::operator-=(const chebfun& f)
  {
    if ( haveSameBase(f) )
    {
      for(size_t i = 0; i < m_p.size(); ++i)
      {
        m_p[i] -= f.m_p[i];
      }
    }
    else
    {
      auto& x = m_base->x;
      for(size_t i = 0; i < m_p.size(); ++i)
      {
        m_p[i] -= f( x[i] );
      }
    }
    invalidateA();
    return *this;
  }

  chebfun& chebfun::operator*=(const chebfun& f)
  {
    if ( haveSameBase(f) )
    {
      for(size_t i = 0; i < m_p.size(); ++i)
      {
        m_p[i] *= f.m_p[i];
      }
    }
    else
    {
      auto& x = m_base->x;
      for(size_t i = 0; i < m_p.size(); ++i)
      {
        m_p[i] *= f( x[i] );
      }
    }
    invalidateA();
    return *this;
  }

  chebfun& chebfun::operator*=(const IFunction& f)
  {
    FunctionDomain1DView x( m_base->x );
    FunctionValues y( x );
    f.function( x, y );
    for(size_t i = 0; i < m_p.size(); ++i)
    {
      m_p[i] *= y[i];
    }
    invalidateA();
    return *this;
  }

  chebfun& chebfun::operator/=(const chebfun& f)
  {
    if ( haveSameBase(f) )
    {
      for(size_t i = 0; i < m_p.size(); ++i)
      {
        m_p[i] /= f.m_p[i];
      }
    }
    else
    {
      auto& x = m_base->x;
      for(size_t i = 0; i < m_p.size(); ++i)
      {
        m_p[i] /= f( x[i] );
      }
    }
    invalidateA();
    return *this;
  }

  /**
   * Create a chebfun from uniform data.
   * @param start :: Start of the interval
   * @param end :: End of the interval.
   * @param p :: Vector with function values to fit to. p[0] is at start, p[n-1] is at end.
   *   Values in between are positioned uniformly in the interval [start,end]. Distance between
   *   adjacent values dx = (end - start) / (n - 1)
   */
  void chebfun::uniformFit(double start, double end, const std::vector<double>& p)
  {
    const double PI = 2*acos(0.0);
    chebfun cheb(p.size() - 1, -1.0, 1.0);
    cheb.setP(p);

    // increase order to improve acuracy
    set(3*p.size(), start, end);
    const double D = end - start;

    auto& x = m_base->x;
    for(size_t i = 0; i < x.size(); ++i)
    {
      double xx = x[i];
      double xx1 = - cos(PI*(xx - start) / D);
      m_p[i] = cheb(xx1);
      //std::cerr << xx << ' ' << xx1 << ' ' << cheb(xx1) -  p[n - 1 - i] << std::endl;
    }
    invalidateA();
  }

  /// Creates a domain for the region on which the workspace is defined.
  Numeric::FunctionDomain1D_sptr chebfun::createDomainFromXPoints() const
  {
    std::vector<double> x;
    x.resize( n() + 1 );

    // ??? why are we copying?
    auto xf = xpoints();
    std::copy(xf.begin(), xf.end(), x.begin() );
    auto domain = new Numeric::FunctionDomain1DVector( x );

    return Numeric::FunctionDomain1D_sptr( domain );
  }

  /**
   * Creates a regular n-point domain for the region on which the workspace is defined.
   */
  Numeric::FunctionDomain1D_sptr chebfun::createDomain(size_t n) const
  {
    return Numeric::FunctionDomain1D_sptr( new Numeric::FunctionDomain1DVector(startX(), endX(), n) );
  }

  /**
   * Modify this chebfun by applying operator op to avlues of this chebfun and chebfun f on domain domain.
   */
  void chebfun::apply(char op, const chebfun& f, const FunctionDomain1D& domain)
  {
    const double start = domain.startX();
    const double end = domain.endX();
    auto& x = m_base->x;
    for(size_t i = 0; i < x.size(); ++i)
    {
      double xx = x[i];
      if ( xx < start || xx > end ) continue;
      const double v = f(xx);
      switch( op )
      {
      case '+': m_p[i] += v; break;
      case '-': m_p[i] -= v; break;
      case '*': m_p[i] *= v; break;
      case '/': m_p[i] /= v; break;
      };
    }
  }


  /// Set the a-coeffs from a std vector
  void chebfun::setA(const std::vector<double>& a)
  {
    const size_t n = a.size();
    if (m_p.size() != n) throw std::runtime_error("chebfun::setA: Wrong size " + 
      boost::lexical_cast<std::string>(m_p.size()) + " " + boost::lexical_cast<std::string>(n)
      );
    m_a.resize( n );
    m_a.assign( a.begin(), a.end() );
    calcP();
  }

  /**
   * Find all roots of this chebfun.
   * @param r :: A vector to store the roots. The vector is resize
   *  to the number of real roots.
   */
  void chebfun::roots(std::vector<double>& r) const
  {
    // build the companion matrix
    auto& a = coeffs();
    size_t N = n();
    // ensure that the highest order coeff is > epsilon
    const double epsilon = std::numeric_limits<double>::epsilon() * 10;
    //std::cerr << "epsilon=" << epsilon << std::endl;
    while( N > 0 && fabs( a[N] ) < epsilon )
    {
      --N;
    }

    if ( N == 0 ) return; // function is a constant

    const size_t N2 = 2*N;
    GSLMatrix C( N2, N2 );
    C.zero();
    const double an = a[N];

    const size_t lasti = N2 - 1;
    for(size_t i = 0; i < N; ++i)
    {
      //std::cerr << i << ' ' << a[i] << std::endl;
      if ( i > 0 )
      {
        C.set( i, i - 1, 1.0 );
      }
      C.set( N + i, N + i - 1, 1.0 );
      C.set( i, lasti, - a[N - i] / an );
      double tmp = - a[i] / an;
      if ( i == 0 ) tmp *= 2;
      C.set( N + i, lasti, tmp );
    }
    std::cerr << N << ' ' << a[N] << std::endl;

    gsl_vector_complex* eval = gsl_vector_complex_alloc( N2 );
    auto workspace = gsl_eigen_nonsymm_alloc( N2 );
    gsl_eigen_nonsymm( C.gsl(), eval, workspace );
    gsl_eigen_nonsymm_free( workspace );

    const double Dx = endX() - startX();
    bool isFirst = true;
    double firstIm = 0;
    for(size_t i = 0; i < N2; ++i )
    {
      auto val = gsl_vector_complex_get( eval, i );
      double re = GSL_REAL( val );
      double im = GSL_IMAG( val );
      double ab = re*re + im*im;
      if ( fabs( ab - 1.0 ) > 1e-5 ) 
      {
        isFirst = true;
        continue;
      }
      //std::cerr << re << ' ' << im << ' ' << re*re + im*im << std::endl;
      if ( isFirst )
      {
        isFirst = false;
        firstIm = im;
      }
      else
      {
        if ( im + firstIm < 1e-10 )
        {
          double x = startX() + ( re + 1.0 ) / 2.0 * Dx;
          r.push_back( x );
        }
        isFirst = true;
      }
    }
    gsl_vector_complex_free( eval );
  }

/**
 * Make a square of this funcion.
 */
void chebfun::square()
{
  for(size_t i = 0; i < m_p.size(); ++i)
  {
    double& d = m_p[i];
    d *= d;
  }
  invalidateA();
}

/**
 * Make a square root of this funcion.
 */
void chebfun::sqrt()
{
  for(size_t i = 0; i < m_p.size(); ++i)
  {
    double& d = m_p[i];
    if ( d < 0.0 )
    {
      throw std::runtime_error("Square root of negative number");
    }
    d = std::sqrt( d );
  }
  invalidateA();
}

void chebfun::bestFit(const IFunction& fun, const double& tol)
{
  BestFit<chebfun,const IFunction&>( *this, fun, tol );
}

/**
 * Resize the base (creates new base with same bounds but different n)
 * @param nn :: New base size.
 */
void chebfun::resize(size_t nn)
{
  if ( m_a.empty() )
  {
    calcA();
  }
  std::vector<double> a( nn + 1, 0.0 );
  size_t dn = 0;
  if ( a.size() < m_a.size() )
  {
    dn = m_a.size() - a.size();
  }
  std::copy( m_a.begin(), m_a.end() - dn, a.begin() );
  set( nn, startX(), endX() );
  setA( a );
}


} // Numeric
