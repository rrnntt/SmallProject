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
  EXPECT_EQ( cheb->fun().nfuns(), 1 );

  //auto& f = cheb->fun( 0 );
  //auto& x = f.xpoints();
  //auto& y = f.ypoints();

  auto domain = cheb->fun().createDomainFromXPoints();
  FunctionValues values( *domain );
  cheb->fun().eval(*domain,values);

  for(size_t i = 0; i < domain->size(); ++i)
  {
    double x = (*domain)[i];
    double y = values.getCalculated(i);
    //std::cerr << x << ' ' << y << ' ' << 2*sin(2*x) << std::endl;
    EXPECT_NEAR( y, 2*sin(2*x), 1e-12 );
  }

  auto ws = API::WorkspaceManager::instance().retrieve("cheb");
  EXPECT_TRUE( ws );
}

