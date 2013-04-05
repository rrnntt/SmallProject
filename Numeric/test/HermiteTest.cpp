#include "gtest/gtest.h"
#include "Numeric/Hermite.h"
#include "Numeric/FunctionDomain1D.h"
#include "Numeric/FunctionValues.h"
#include "Numeric/GSLMatrix.h"

using namespace Numeric;

double test_integrate(const std::vector<double>& r, const std::vector<double>& w, const IFunction& fun)
{
  FunctionDomain1DView x( r );
  FunctionValues y( x );
  fun.function( x, y );
  double sum = 0.0;
  for(size_t i = 0; i < x.size(); ++i)
  {
    sum += y[i] * w[i];
    //std::cerr << "   *** " << y[i] << ' ' << w[i] << std::endl;
  }
  return sum;
}

double test_integrate2(const std::vector<double>& r, const std::vector<double>& w, const IFunction& fun)
{
  FunctionDomain1DView x( r );
  FunctionValues y( x );
  fun.function( x, y );
  double sum = 0.0;
  for(size_t i = 0; i < x.size(); ++i)
  {
    sum += y[i] * y[i] * w[i];
  }
  return sum;
}

double test_integrate2T(const std::vector<double>& r, const GSLMatrix& T, const IFunction& fun)
{
  FunctionDomain1DView x( r );
  FunctionValues y( x );
  fun.function( x, y );
  double sum = 0.0;
  for(size_t i = 0; i < x.size(); ++i)
  for(size_t k = 0; k < x.size(); ++k)
  {
    sum += y[i] * y[k] * T.get(i,k);
  }
  return sum;
}

TEST(Numeric_Hermite_Test, WeightsTest)
{
  const size_t n = 10;
  Hermite H( n );
  auto& r = H.getRoots();
  auto& w = H.getWeights();

  double sum = 0.0;
  for(size_t i = 0; i < n; ++i)
  {
    sum += w[i];
  }
  EXPECT_NEAR( sum, H.weightIntegral(), 1e-10 );


}

TEST(Numeric_Hermite_Test, PartialQuadTest)
{
  Hermite L(10);

  auto& r = L.getRoots();
  auto& w = L.getWeights();

  std::set<size_t> ri;
  ri.insert(3);
  ri.insert(4);
  ri.insert(5);
  ri.insert(6);
  size_t m = ri.size();
  std::vector<double> rp; // partial roots
  std::vector<double> wp; // partial weights

  //system("pause");
  GSLMatrix s;
  GSLMatrix T;
  L.partialQuadrature( ri, rp, wp, &s, &T );

  for(size_t i = 0; i < wp.size(); ++i)
  {
    std::cerr << i << ' ' << rp[i] << ' ' << wp[i] << std::endl;
  }

  std::cerr << "H0 " << test_integrate(rp,wp,Hermite(0)) << std::endl;
  std::cerr << "H1 " << test_integrate(rp,wp,Hermite(1)) << std::endl;
  std::cerr << "H2 " << test_integrate(rp,wp,Hermite(2)) << std::endl;
  std::cerr << "H3 " << test_integrate(rp,wp,Hermite(3)) << std::endl;
  std::cerr << "H4 " << test_integrate(rp,wp,Hermite(4)) << std::endl;
  std::cerr << "H5 " << test_integrate(rp,wp,Hermite(5)) << std::endl;

  std::cerr << "H0 " << test_integrate2(rp,wp,Hermite(0)) << ' ' << test_integrate2(r,w,Hermite(0)) << std::endl;
  std::cerr << "H1 " << test_integrate2(rp,wp,Hermite(1)) << ' ' << test_integrate2(r,w,Hermite(1)) << std::endl;
  std::cerr << "H2 " << test_integrate2(rp,wp,Hermite(2)) << ' ' << test_integrate2(r,w,Hermite(2)) << std::endl;
  std::cerr << "H3 " << test_integrate2(rp,wp,Hermite(3)) << ' ' << test_integrate2(r,w,Hermite(3)) << std::endl;

  std::cerr << T.size1() << ' ' << T.size2() << std::endl;
  std::cerr << T ;
  std::cerr << "H2T " << test_integrate2T(rp,T,Hermite(2)) << ' ' << test_integrate2(r,w,Hermite(2)) << std::endl;
  std::cerr << "H3T " << test_integrate2T(rp,T,Hermite(3)) << ' ' << test_integrate2(r,w,Hermite(3)) << std::endl;
  std::cerr << "H4T " << test_integrate2T(rp,T,Hermite(4)) << ' ' << test_integrate2(r,w,Hermite(4)) << std::endl;

}

TEST(Numeric_Hermite_Test, PartialQuad3Test)
{
  const size_t nq = 10;
  Hermite L(nq);

  auto& r = L.getRoots();
  auto& w = L.getWeights();

  const size_t mq = 3;
  Hermite L1(mq);

  std::vector<double> rp; // partial roots
  //rp = L1.getRoots();
  rp.push_back( r[4] );
  rp.push_back( r[5] );
  rp.push_back( r[6]+0.33 );
  rp.push_back( r[3]-0.5 );
  std::vector<double> wp; // partial weights

  //system("pause");
  GSLMatrix s;
  GSLMatrix T;
  L.partialQuadrature3( rp, wp, &s, &T );

  std::cerr << "======= big ========" << std::endl;
  for(size_t i = 0; i < r.size(); ++i)
  {
    std::cerr << i << ' ' << r[i] << ' ' << w[i] << std::endl;
  }

  std::cerr << "======= small ========" << std::endl;
  for(size_t i = 0; i < rp.size(); ++i)
  {
    std::cerr << i << ' ' << rp[i] << ' ' << wp[i] << std::endl;
  }

  std::cerr << "=======  T  ========" << std::endl;
  std::cerr << T << std::endl;

  std::cerr << "H1^2 " << test_integrate2(rp,wp,Hermite(1)) << ' ' << test_integrate2(r,w,Hermite(1)) << std::endl;
  std::cerr << "H2   " << test_integrate(rp,wp,Hermite(2)) << ' ' << test_integrate(r,w,Hermite(2)) << std::endl;
  std::cerr << "H3   " << test_integrate(rp,wp,Hermite(3)) << ' ' << test_integrate(r,w,Hermite(3)) << std::endl;
  std::cerr << "H2^2 " << test_integrate2(rp,wp,Hermite(2)) << ' ' << test_integrate2(r,w,Hermite(2)) << std::endl;
  std::cerr << "H4   " << test_integrate(rp,wp,Hermite(4)) << ' ' << test_integrate(r,w,Hermite(4)) << std::endl;
  std::cerr << "H5   " << test_integrate(rp,wp,Hermite(5)) << ' ' << test_integrate(r,w,Hermite(5)) << std::endl;
  std::cerr << "H6   " << test_integrate(rp,wp,Hermite(6)) << ' ' << test_integrate(r,w,Hermite(6)) << std::endl;
  std::cerr << "H3^2 " << test_integrate2(rp,wp,Hermite(3)) << ' ' << test_integrate2(r,w,Hermite(3)) << std::endl;

}
