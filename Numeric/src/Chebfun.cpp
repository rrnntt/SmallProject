#include "Numeric/Chebfun.h"

#include <gsl/gsl_errno.h>
#include <gsl/gsl_fft_real.h>
#include <gsl/gsl_integration.h>
//#include <gsl/gsl_fft_halfcomplex.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <iostream>

namespace Numeric
{
  chebfun::chebfun(int n, const double& startX,const double& endX)
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
  m_n(other.m_n),m_startX(other.m_startX),m_endX(other.m_endX),m_x(other.m_x),m_w(other.m_w)
  {
    m_p.assign(other.m_p.begin(),other.m_p.end());
    m_a.assign(other.m_a.begin(),other.m_a.end());
  }

  /**
    * Create a copy of the other. x array is shared.
    */
  chebfun& chebfun::operator=(const chebfun& other)
  {
    m_startX = other.m_startX;
    m_endX = other.m_endX;
    m_x = other.m_x;
    m_w = other.m_w;
    m_p.assign(other.m_p.begin(),other.m_p.end());
    m_a.assign(other.m_a.begin(),other.m_a.end());
    return *this;
  }

  /**
    * Resets the x array (if it isn't shared)
    */
  void chebfun::set(int n,const double& startX,const double& endX)
  {
    m_startX = startX;
    m_endX = endX;
    if (!m_x || m_x.unique())
    {
      m_n = n;
      m_a.resize(n+1);
      calcX();
    }
    else
    {
      throw std::runtime_error("Cannot resize shared chebfun x vector");
    }
  }

  void chebfun::setStartX(const double& d)
  {
    if (!m_x || m_x.unique())
    {
      m_startX = d;
      calcX();
    }
    else
    {
      throw std::runtime_error("Cannot reset start x in shared chebfun");
    }
  }

  void chebfun::setEndX(const double& d)
  {
    if (!m_x || m_x.unique())
    {
      m_endX = d;
      calcX();
    }
    else
    {
      throw std::runtime_error("Cannot reset end x in shared chebfun");
    }
  }

  void chebfun::setRange(const double& s,const double& e)
  {
    if (!m_x || m_x.unique())
    {
      m_startX = s;
      m_endX = e;
      calcX();
    }
    else
    {
      throw std::runtime_error("Cannot reset x bounds in shared chebfun");
    }
}

  /**
    * Calculate the function value as a T form expansion at point x
    * @param x :: Argument of the function
    */
  double chebfun::valueT(const double& x)const
  {
    double c = 2./(m_endX - m_startX);
    double a = 1. - c * m_endX;
    // scale to interval -1 < x < 1
    double xx = a + c * x;
    double b2 = 0.;
    double b1 = 0.;
    double b;
    for(int j = m_n; j > 0; --j)
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
    double c = 2./(m_endX - m_startX);
    double a = 1. - c * m_endX;
    // scale to interval -1 < x < 1
    double xx = a + c * x;
    double b2 = 0.;
    double b1 = 0.;
    double b;
    for(int j = m_n; j > 0; --j)
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
    double c = 2./(m_endX - m_startX);
    double a = 1. - c * m_endX;
    // scale to interval -1 < x < 1
    double xx = a + c * x;
    double b2 = 0.;
    double b1 = 0.;
    double b;
    for(int j = m_n - 1; j > 0; --j)
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
    double n2 = m_n*m_n;
    double res = n2*(n2-1)/3;
    if ( m_n % 2 != 0 ) res = -res;
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
   * Calculate the m_x array as zeroes of the m_n-th order chebyshev polynomial.
   * If the array is shared x values will change for all chebfuns
   */
  void chebfun::calcX()
  {
    if (!m_x || !m_w)
    {
      m_x.reset(new std::vector<double>);
      m_w.reset(new std::vector<double>);
    }

    m_x->resize(m_n+1);
    m_w->resize(m_n+1);
    m_p.resize(m_n+1);
    double x0 = (m_startX + m_endX)/2;
    double b  = (m_endX - m_startX)/2;
    for(size_t i = 0; i < m_x->size(); ++i)
    {
      int j = m_x->size() - 1 - i;
      (*m_x)[i] = x0 + b * cos( M_PI*i/m_n );
      (*m_w)[i] = 1.;
      if ( i % 2 != 0 ) (*m_w)[i] = -1.;
      if ( i == 0 || i == m_n ) (*m_w)[i] /= 2;
      //std::cerr<<"x["<<i<<"]="<<m_x[i]<<'\n';
    }
  }

  /**
   * Calculate the value using teh barycenteric interpolation formula
   */
  double chebfun::valueB(const double& x)const
  {
    std::vector<double>::const_iterator it = 
      std::find(m_x->begin(),m_x->end(),x);
    if (it != m_x->end())
    {
      std::vector<double>::difference_type i = std::distance(std::vector<double>::const_iterator(m_x->begin()),it);
      return m_p[i];
    }
    double res = 0;
    double weight = 0;
    for(size_t i=0;i<m_x->size();++i)
    {
      double W = (*m_w)[i]/(x - (*m_x)[i]);
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
        int m_size;           ///< size of the transformed data
        double* m_data; ///< pointer to the transformed data
        bool m_even;          ///< true if the size of the original data is even
      public:
        /**
         * Constructor. 
         * @param data A pointer to the transformed complex data
         * @param n The size of untransformed real data
         */
        HalfComplex(double* data,const int& n):m_size(n/2+1),m_data(data),m_even(n/2*2==n)
        {
        }
        /// Returns the size of the transform
        int size()const{return m_size;}
        /**
         * The real part of i-th transform coefficient
         * @param i The index of the complex transform coefficient
         * @return The real part 
         */
        double real(int i)const
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
        double imag(int i)const
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
        void set(int i,const double& re,const double& im)
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
  void chebfun::calcA()
  {
    int n = m_n+1;
    m_a.resize(n);

    //// This is a correct and direct transform from m_p to m_a
    //// DO NOT DELETE !!!
    //for(int i=0;i<n;++i)
    //{
    //  double t = 0.;
    //  for(int j=0;j<=m_n;j++)
    //  {
    //    double p = m_p[j];
    //    if (j== 0 || j == m_n) p /= 2;
    //    t += cos(M_PI*i*(double(j))/m_n)*p;
    //  }
    //  m_a[i] = 2*t/m_n;
    //  if (i == 0) m_a[0] /= 2;
    //}
    //// End of the correct and direct transform from m_p to m_a

    // This is a magic trick which uses real fft to do the above cosine transform
    std::vector<double> tmp(m_n*2);
    std::copy(m_p.begin(),m_p.end(),tmp.begin());
    std::reverse_copy(m_p.begin()+1,m_p.end()-1,tmp.begin()+m_n+1);

    gsl_fft_real_workspace * workspace = gsl_fft_real_workspace_alloc(2*m_n);
    gsl_fft_real_wavetable * wavetable = gsl_fft_real_wavetable_alloc(2*m_n);
    gsl_fft_real_transform (&tmp[0], 1, 2*m_n, wavetable, workspace);
    gsl_fft_real_wavetable_free (wavetable);
    gsl_fft_real_workspace_free (workspace);

    HalfComplex fc(&tmp[0],tmp.size());
    for(size_t i=0;i<n;++i)
    {
      double a = fc.real(i)/m_n;
      if (i == 0) a /= 2;
      m_a[i] = a;
    }
    // End of the magic trick
  }

  void chebfun::calcP()
  {
    if (m_p.size() != m_a.size())
    {
      m_p.resize(m_a.size());
    }
    for(size_t i = 0; i < m_a.size(); ++i)
    {
      m_p[i] = valueT((*m_x)[i]);
    }
  }

  /// make this chebfun a derivative of the argument
  void chebfun::fromDerivative(const chebfun& fun)
  {
    m_n = fun.m_n;
    m_x = fun.m_x; // shared
    m_w = fun.m_w; // shared
    m_startX = fun.m_startX;
    m_endX = fun.m_endX;
    double dx = 1e-10*(m_endX - m_startX);
    std::vector<double> tmp(m_n+1);
    m_p.resize(m_n+1);
    for(size_t i = 0; i < m_p.size(); ++i)
    {
      tmp[i] = (fun.valueB((*m_x)[i] + dx) - fun.m_p[i]) / dx;
    }
    std::copy(tmp.begin(),tmp.end(),m_p.begin());
    calcA(); // ?
  }

  double chebfun_integration_function(double x,void* this_chebfun)
  {
    const chebfun& fun = *static_cast<const chebfun*>(this_chebfun);
    return fun(x);
  }

  double chebfun_integration_function2(double x,void* this_chebfun)
  {
    const chebfun& fun = *static_cast<const chebfun*>(this_chebfun);
    double f = fun(x);
    return f*f;
  }

  double chebfun::integrate(int pwr)
  {
    gsl_integration_glfixed_table * quad = gsl_integration_glfixed_table_alloc(m_n);
    gsl_function fun;
    fun.function = pwr == 1 ? chebfun_integration_function : chebfun_integration_function2;
    fun.params = this;
    double res = gsl_integration_glfixed (&fun, m_startX,m_endX, quad);
    gsl_integration_glfixed_table_free(quad);
    return res;
  }

} // Numeric

