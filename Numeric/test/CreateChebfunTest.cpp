#include "gtest/gtest.h"
#include "Numeric/CreateChebfun.h"
#include "Numeric/ChebfunWorkspace.h"
#include "API/WorkspaceManager.h"

#include <iostream>

using namespace Numeric;

TEST(CreateChebfunTest, Test)
{
  CreateChebfun alg;
  alg.setProperty("Function", "UserFunction1D(Formula=2*sin(2*x))");
  alg.setProperty("Chebfun", "cheb");
  alg.execute();

  ChebfunWorkspace_sptr cheb = alg.getClass("Chebfun");
  EXPECT_TRUE( cheb );
  EXPECT_EQ( cheb->nfuns(), 1 );

  auto& f = cheb->fun( 0 );
  auto& x = f.xpoints();
  auto& y = f.ypoints();

  for(size_t i = 0; i < x.size(); ++i)
  {
    //std::cerr << x[i] << ' ' << y[i] << ' ' << 2*sin(2*x[i]) << std::endl;
    EXPECT_NEAR( y[i], 2*sin(2*x[i]), 1e-12 );
  }

  auto ws = API::WorkspaceManager::instance().retrieve("cheb");
  EXPECT_TRUE( ws );
}

