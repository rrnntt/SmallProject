#include "gtest/gtest.h"
#include "Numeric/LevenbergMarquardt.h"
#include "Numeric/FunctionDomain1D.h"
#include "Numeric/FunctionValues.h"
#include "Numeric/UserFunction1D.h"
#include "Numeric/LeastSquares.h"

using namespace Numeric;

TEST(Numeric_LevenbergMarquardt_Test, Test)
{
    FunctionDomain1D_sptr domain(new FunctionDomain1DVector(0.0,10.0,20));
    FunctionValues mockData(*domain);
    UserFunction1D dataMaker;
    dataMaker.setAttributeValue("Formula","a*x+b");
    dataMaker.setParameter("a",1.1);
    dataMaker.setParameter("b",2.2);
    dataMaker.function(*domain,mockData);

    FunctionValues_sptr values(new FunctionValues(*domain));
    values->setFitDataFromCalculated(mockData);
    values->setFitWeights(1.0);

    boost::shared_ptr<UserFunction1D> fun(new UserFunction1D);
    fun->setAttributeValue("Formula","a*x+b");
    fun->setParameter("a",1.);
    fun->setParameter("b",2.);

    boost::shared_ptr<LeastSquares> costFun(new LeastSquares);
    costFun->setFittingFunction(fun,domain,values);
    EXPECT_EQ(costFun->nParams(),2);

    LevenbergMarquardt s;
    s.initialize(costFun);
    EXPECT_TRUE(s.minimize());

    EXPECT_NEAR( fun->getParameter("a"), 1.1, 0.01 );
    EXPECT_NEAR( fun->getParameter("b"), 2.2, 0.01 );
    EXPECT_EQ( s.getError(), "success" );
}
