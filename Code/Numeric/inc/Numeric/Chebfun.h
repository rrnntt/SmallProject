#ifndef NUMERIC_CHEBFUN_H
#define NUMERIC_CHEBFUN_H

#include "Numeric/DllExport.h"
#include "Numeric/FunctionDomain1D.h"
#include "Numeric/FunctionValues.h"
#include "Numeric/GSLVector.h"
#include "Numeric/GSLMatrix.h"
#include "Numeric/Constants.h"

#include <vector>
#include <boost/shared_ptr.hpp>

typedef double (*AFunction)(double);

namespace Numeric
{
  class IFunction;

  /**
   * A base of a chebfun - its domain. Can be shared between chebfuns.
   */
  struct NUMERIC_EXPORT ChebfunBase
  {
    ChebfunBase(size_t k, double s, double e):n(k),startX(s),endX(e){calcX();}
    size_t n;              ///< polynomial order
    double startX, endX;   ///< set boundaries where the function is defined
    size_t size() const {return x.size();} ///< number of x-points == n + 1
    std::vector<double> x;   ///< x-vaues for use in the barycentric formula, n + 1 items
    std::vector<double> w;   ///< weights used in the barycentric formula, n + 1 items
    std::vector<double> iw;   ///< weights for integration using a-coeffs
    std::vector<double> iw2;   ///< weights for weighted integration
    std::vector<double> iw3;   ///< weights for integration using p values
    void calcX();  ///< Calclulate x and w based on values of n, startX, and endX
  };

  typedef boost::shared_ptr<ChebfunBase> ChebfunBase_sptr;
  typedef boost::shared_ptr<const ChebfunBase> ChebfunBase_const_sptr;

  /**
   * A chebfun
   */
  class NUMERIC_EXPORT chebfun
  {
  public:
    chebfun(size_t n = 0, const double& startX = -1,const double& endX = 1);
    chebfun(const chebfun& other);
    chebfun& operator=(const chebfun& other);
    ChebfunBase_const_sptr getBase() const {return m_base;}
    void setBase(const chebfun& other){m_base = other.m_base; m_p.resize( n() + 1 ); invalidateA();}
    void setBase(ChebfunBase_sptr base){m_base = base; m_p.resize( n() + 1 ); invalidateA();}
    /// Check if two chebfuns have shared x-points.
    bool haveSameBase(const chebfun& other) const {return m_base == other.m_base;}
    /// Is the chebfun valid and set
    bool isValid() const {return static_cast<bool>(m_base);}
    /// Order of the polynomial
    size_t n()const{return m_base->n;}
    void set(size_t n,const double& startX = -1,const double& endX = 1);
    double startX()const{return m_base->startX;}
    double endX()const{return m_base->endX;}
    /// Get the vector of x-points, size n() + 1
    const std::vector<double>& xpoints()const{return m_base->x;}
    /// Get the vector of y-points, size n() + 1
    const std::vector<double>& ypoints()const{return m_p;}
    /// Get the vector of y-points, size n() + 1
    const std::vector<double>& coeffs()const{if (m_a.empty()) calcA(); return m_a;}
    /// Get the vector of y-points, size n() + 1
    std::vector<double>& coeffs(){if (m_a.empty()) calcA(); return m_a;}
    /// Set the y points from a std vector
    const std::vector<double>& weights()const{return m_base->iw2;}
    /// Get the vector of y-points, size n() + 1
    void setP(const std::vector<double>& y);
    /// Set the y points from a GSL vector
    void setP(const GSLVector& y);
    /// Set individual y point
    void setP(size_t i, double y);
    /// Set the a-coeffs from a std vector
    void setA(const std::vector<double>& a);
    /// Set the y points from a column of a GSL matrix
    void setP(const GSLMatrix& M, size_t col);
    void fit(const IFunction& ifun);
    void fit(AFunction f);
    template<typename TYPE> void bestFit(TYPE fun, const double& tol = 1e-16);
    void bestFit(const IFunction& fun, const double& tol = 1e-16);
    void uniformFit(double start, double end, const std::vector<double>& p);
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
    /// make this chebfun a second derivative of the argument
    void fromDerivative2(const chebfun& fun);
    /// Resize the base (creates new base with same bounds but different n)
    void resize(size_t nn);

    chebfun& operator=(AFunction f)
    {
      fit(f);
      return *this;
    }
    chebfun& operator=(const double& f)
    {
      std::fill(m_p.begin(),m_p.end(),f);
      return *this;
    }

    /// += operators
    chebfun& operator+=(double v)
    {
      std::transform(m_p.begin(),m_p.end(),m_p.begin(),std::bind1st(std::plus<double>(),v));
      invalidateA();
      return *this;
    }
    chebfun& operator+=(int v){return *this+=double(v);}
    chebfun& operator+=(AFunction f)
    {
      auto& x = m_base->x;
      for(size_t i = 0; i < m_p.size(); ++i)
      {
        m_p[i] += f( x[i] );
      }
      invalidateA();
      return *this;
    }
    
    chebfun& operator+=(const chebfun& f);

    /// -= operators
    chebfun& operator-=(double v)
    {
      std::transform(m_p.begin(),m_p.end(),m_p.begin(),std::bind2nd(std::minus<double>(),v));
      invalidateA();
      return *this;
    }
    chebfun& operator-=(int v){return *this-=double(v);}
    chebfun& operator-=(AFunction f)
    {
      auto& x = m_base->x;
      for(size_t i = 0; i < m_p.size(); ++i)
      {
        m_p[i] -= f( x[i] );
      }
      invalidateA();
      return *this;
    }
    
    chebfun& operator-=(const chebfun& f);

    /// *= operators
    chebfun& operator*=(double v)
    {
      std::transform(m_p.begin(),m_p.end(),m_p.begin(),std::bind2nd(std::multiplies<double>(),v));
      invalidateA();
      return *this;
    }
    chebfun& operator*=(int v){return *this*=double(v);}
    chebfun& operator*=(AFunction f)
    {
      auto& x = m_base->x;
      for(size_t i = 0; i < m_p.size(); ++i)
      {
        m_p[i] *= f( x[i] );
      }
      invalidateA();
      return *this;
    }
    
    chebfun& operator*=(const chebfun& f);
    chebfun& operator*=(const IFunction& f);

    /// /= operators
    chebfun& operator/=(double v)
    {
      std::transform(m_p.begin(),m_p.end(),m_p.begin(),std::bind2nd(std::divides<double>(),v));
      invalidateA();
      return *this;
    }
    chebfun& operator/=(int v){return *this/=double(v);}
    chebfun& operator/=(AFunction f)
    {
      auto& x = m_base->x;
      for(size_t i = 0; i < m_p.size(); ++i)
      {
        m_p[i] /= f( x[i] );
      }
      invalidateA();
      return *this;
    }
    
    chebfun& operator/=(const chebfun& f);

    /// 
    void apply(char op, const chebfun& f, const FunctionDomain1D& domain);

    /// Creates a domain for the region on which the workspace is defined.
    Numeric::FunctionDomain1D_sptr createDomainFromXPoints() const;
    /// Creates a domain for the region on which the workspace is defined.
    Numeric::FunctionDomain1D_sptr createDomain(size_t n) const;

    double integrate();
    double integr() const;
    /// Integrate this function with weight sqrt(1-x^2)
    double integrateWeighted() const;
    double norm2();
    std::vector<double> baseNorm() const;

    void square();
    void sqrt();
    void convolution(const IFunction &fun1, const IFunction &fun2, double start, double end, bool inverse = false);

    void roots(std::vector<double>& r) const;

    void calcP(); ///< calc m_p form m_a
    void calcA() const; ///< calc m_a from m_p
  protected:

    void invalidateA() { m_a.clear(); }

    boost::shared_ptr< ChebfunBase > m_base; ///< The base of a chebfun holding x-values and weights
    std::vector<double> m_p; ///< function values at m_x points, m_n + 1 items
    mutable std::vector<double> m_a; ///< polynomial coefficients, m_n + 1 items
  };

  /**
   * Find the number on x-points (n) large enough to reproduce fun
   * with accuracy ~1e-16.
   */
  template<class CHEB, typename TYPE> 
  void BestFit(CHEB& cheb, TYPE fun, const double tol = 1e-16)
  {
    const double start = cheb.startX();
    const double end = cheb.endX();
    size_t nn = 3;
    const size_t nMax = 5000;
    double err = 1.0;
    double maxA = 0;
    while ( err > tol )
    {
      cheb.set( nn, start, end );
      cheb.fit( fun );
      auto& a = cheb.coeffs();
      double minAodd = inf;
      double minAeven = inf;
      for(size_t i = 0; i < a.size(); i+=2)
      {
        const double abs_even = fabs( a[i] );
        const double abs_odd = fabs( a[i+1] );
        if ( abs_even < minAeven ) minAeven = abs_even;
        if ( abs_odd < minAodd ) minAodd = abs_odd;
        if ( abs_even > maxA ) maxA = abs_even;
        if ( abs_odd > maxA ) maxA = abs_odd;
      }
      err = (minAodd + minAeven) / maxA / 2;
      nn *= 2;
      --nn;
      if ( nn > nMax ) throw std::runtime_error("chebfun fit cannot converge.");
    }
    // optimize the fit by finding the smallest significant a-coefficient
    auto& a = cheb.coeffs();
    for(auto ia = a.rbegin(); ia != a.rend(); ++ia)
    {
      //std::cerr << "* " << a.rend() - ia << ' ' << *ia << std::endl;
      if ( fabs(*ia) / maxA >= tol )
      {
        if ( ia != a.rbegin() )
        {
          // this n is optimal
          size_t n = static_cast<size_t>(a.rend() - ia) - 1;
          cheb.set( n, start, end );
          cheb.fit( fun );
        }
        break;
      }
    }
  }

  /**
   * Find the number on x-points (n) large enough to reproduce fun
   * with accuracy ~1e-16.
   */
  template<typename TYPE> 
  void chebfun::bestFit(TYPE fun, const double& tol)
  {
    BestFit( *this, fun, tol );
  }

  typedef boost::shared_ptr<chebfun> chebfun_sptr;
  typedef boost::shared_ptr<const chebfun> chebfun_const_sptr;

} // Numeric


#endif /* NUMERIC_CHEBFUN_H */
