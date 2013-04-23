#include "gtest/gtest.h"
#include "Numeric/Chebfun2DSpline.h"
#include "Numeric/FunctionDomain1D.h"

#include <fstream>

using namespace Numeric;

double test_function(double x, double y)
{
  return exp(-(x*x + y*y));
}

TEST(Numeric_Chebfun2DSpline_Test, Test)
{
  Chebfun2DSpline spline(100,-3,3,4,100,-3,3,4);
  spline.fit( test_function );


  std::ofstream fil("spline.txt");
  fil << "#double:X\n";
  fil << "#double:Y\n";
  fil << "#double:Y1\n";
  fil << "#double:D\n";

  FunctionDomain1DVector var(-3,3,100);
  for(size_t i = 0; i < var.size(); ++i)
  {
  const double x = var[i];
  const double y = var[i];
  std::cerr << spline(x,y) << ' ' << test_function(x, y) << std::endl;
  double s = spline(x, y);
  double t = test_function(x, y);
  fil << x << ' ' << s << ' ' << t << ' ' << t - s << std::endl;
  }
}
