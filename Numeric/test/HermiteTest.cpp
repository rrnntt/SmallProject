#include "gtest/gtest.h"
#include "Numeric/Hermite.h"
#include "Numeric/FunctionDomain1D.h"
#include "Numeric/FunctionValues.h"

using namespace Numeric;

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

