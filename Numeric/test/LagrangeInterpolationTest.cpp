#include "gtest/gtest.h"
#include "Numeric/LagrangeInterpolation.h"

using namespace Numeric;

TEST(Numeric_LagrangeInterpolation_Test, Test)
{
  std::vector<double> r;
  r.push_back( 0.0 );
  r.push_back( 1.0 );
  r.push_back( 2.0 );
  LagrangeInterpolation L( r );
  L.setValue( 0, 1.0 );
  L.setValue( 1, 2.0 );
  L.setValue( 2, 3.0 );
}
