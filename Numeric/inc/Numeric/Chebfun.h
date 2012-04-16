#ifndef NUMERIC_CHEBFUN_H
#define NUMERIC_CHEBFUN_H

#include "Numeric/DllExport.h"

#include <vector>
#include <boost/shared_ptr.hpp>

typedef double (*AFunction)(double);

namespace Numeric
{
  class IFunction;

  class NUMERIC_EXPORT chebfun
  {
  public:
    chebfun(int n = -1, const double& startX = -1,const double& endX = 1);
    chebfun(const chebfun& other);
    chebfun& operator=(const chebfun& other);
    int n()const{return m_n;}
    void set(int n,const double& startX = -1,const double& endX = 1);
    double startX()const{return m_startX;}
    void setStartX(const double& d);
    double endX()const{return m_endX;}
    void setEndX(const double& d);
    void setRange(const double& s,const double& e);
    double& param(size_t i){return m_a.at(i);}
    const std::vector<double>& xpoints()const{return *m_x;}
    const std::vector<double>& ypoints()const{return m_p;}
    void fit(AFunction f){fit<AFunction>(f);}
    void fit(const IFunction& ifun);
    template<class T>
    void fit(T f)
    {
      m_p.resize(m_x->size());
      calcX();
      for(size_t i=0;i<m_x->size();++i)
      {
        m_p[i] = f((*m_x)[i]);
      }
      calcA();
    }
    /// calculate value at point x using the m_a's and treating them as expansion coefficients over Chebyshev T polynomials
    double valueT(const double& x)const;
    /// calculate value at point x using the m_a's and treating them as expansion coefficients over Chebyshev U polynomials
    double valueU(const double& x)const;
    /// calculate the first derivative at x in the T-form
    double derivT(const double& x)const;
    /// calculate the second derivative at x in the T-form (not implemented yet)
    double derivT2(const double& x)const;
    /// calculate value at point x using the barycentric formula and hence vectors m_x,m_p and m_w. T-form
    double valueB(const double& x)const;
    /// evaluate the function (as valueB(x))
    double operator()(const double& x)const{return valueB(x);}
    /// make this chebfun a derivative of the argument
    void fromDerivative(const chebfun& fun);

    chebfun& operator=(AFunction f)
    {
      fit(f);
      return *this;
    }
    template<class T>
    chebfun& operator=(const T& f)
    {
      std::fill(m_p.begin(),m_p.end(),0.0);
      (*this)+=f;
      return *this;
    }

    /// += operators
    chebfun& operator+=(double v)
    {
      std::transform(m_p.begin(),m_p.end(),m_p.begin(),std::bind1st(std::plus<double>(),v));
      calcA();
      return *this;
    }
    chebfun& operator+=(int v){return *this+=double(v);}
    template<class T>
    chebfun& operator+=(const T& f)
    {
      for(size_t i = 0; i < m_p.size(); ++i)
      {
        m_p[i] += f((*m_x)[i]);
      }
      calcA();
      return *this;
    }
    chebfun& operator+=(AFunction f){return this->operator+=<AFunction>(f);}
    chebfun& operator+=(const chebfun& f);

    /// -= operators
    chebfun& operator-=(double v)
    {
      std::transform(m_p.begin(),m_p.end(),m_p.begin(),std::bind2nd(std::minus<double>(),v));
      calcA();
      return *this;
    }
    chebfun& operator-=(int v){return *this-=double(v);}
    template<class T>
    chebfun& operator-=(T f)
    {
      for(size_t i = 0; i < m_p.size(); ++i)
      {
        m_p[i] -= f((*m_x)[i]);
      }
      calcA();
      return *this;
    }
    chebfun& operator-=(AFunction f){return this->operator-=<AFunction>(f);}
    chebfun& operator-=(const chebfun& f);

    /// *= operators
    chebfun& operator*=(double v)
    {
      std::transform(m_p.begin(),m_p.end(),m_p.begin(),std::bind2nd(std::multiplies<double>(),v));
      calcA();
      return *this;
    }
    chebfun& operator*=(int v){return *this*=double(v);}
    template<class T>
    chebfun& operator*=(T f)
    {
      for(size_t i = 0; i < m_p.size(); ++i)
      {
        m_p[i] *= f((*m_x)[i]);
      }
      calcA();
      return *this;
    }
    chebfun& operator*=(AFunction f){return this->operator*=<AFunction>(f);}
    chebfun& operator*=(const chebfun& f);

    /// /= operators
    chebfun& operator/=(double v)
    {
      std::transform(m_p.begin(),m_p.end(),m_p.begin(),std::bind2nd(std::divides<double>(),v));
      calcA();
      return *this;
    }
    chebfun& operator/=(int v){return *this/=double(v);}
    template<class T>
    chebfun& operator/=(T f)
    {
      for(size_t i = 0; i < m_p.size(); ++i)
      {
        m_p[i] /= f((*m_x)[i]);
      }
      calcA();
      return *this;
    }
    chebfun& operator/=(AFunction f){return this->operator/=<AFunction>(f);}
    chebfun& operator/=(const chebfun& f);

    double integrate(int pwr = 1);

    void calcP(); ///< calc m_p form m_a
  private:

    void calcX();
    void calcA(); ///< calc m_a from m_p

    int m_n;                 ///< polynomial order
    double m_startX,m_endX;  ///< set boundaries where the function is defined
    boost::shared_ptr< std::vector<double> > m_x; ///< x-vaues for use in the barycentric formula, m_n + 1 items
    boost::shared_ptr< std::vector<double> > m_w; ///< some weights used in the barycentric formula, m_n + 1 items
    std::vector<double> m_p; ///< function values at m_x points, m_n + 1 items
    std::vector<double> m_a; ///< polynomial coefficients, m_n + 1 items
    friend class chebfunTest;
  };

  typedef boost::shared_ptr<chebfun> chebfun_ptr;
  typedef boost::shared_ptr<const chebfun> chebfun_const_ptr;

} // Numeric


#endif /* NUMERIC_CHEBFUN_H */
