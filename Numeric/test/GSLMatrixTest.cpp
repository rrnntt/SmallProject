#include "gtest/gtest.h"
#include "Numeric/GSLMatrix.h"

using namespace Numeric;

TEST(Numeric_GSLMatrix_Test, DiagTest)
{
  GSLMatrix M(3,3);
  M.set(0,0,1.0);
  M.set(0,1,0.1);
  M.set(0,2,0.2);
  M.set(1,0,0.1);
  M.set(1,1,2.0);
  M.set(1,2,1.2);
  M.set(2,0,0.2);
  M.set(2,1,1.2);
  M.set(2,2,3.0);

  GSLMatrix M0 = M;

  GSLVector d;
  GSLMatrix v;

  M.diag( d, v );

  GSLMatrix T;
  T = Tr(v) * M0 * v;

  for(size_t i = 0; i < d.size(); ++i)
  {
    EXPECT_NEAR( d[i], T.get(i, i), 1e-15 );
  }
}
