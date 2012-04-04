#include "gtest/gtest.h"
#include "Numeric/IFunction1D.h"
#include "Numeric/ParamFunction.h"
#include "Numeric/CompositeFunction.h"
#include "Numeric/FunctionFactory.h"

#include <iostream>

using namespace Numeric;

class FunctionFactoryTestFunction: public virtual IFunction1D, public virtual ParamFunction
{
public:
  FunctionFactoryTestFunction()
  {
    declareParameter("A",1.0);
    declareParameter("B",-3.0);
  }
  virtual std::string name()const {return "FunctionFactoryTestFunction";}
protected:
  virtual void function1D(double* out, const double* xValues, const size_t nData)const
  {
    double A = getParameter(0);
    double B = getParameter(1);
    for(size_t i = 0; i < nData; ++i)
    {
      double x = xValues[i];
      out[i] = A*x+B;
    }
  }
};

DECLARE_FUNCTION(FunctionFactoryTestFunction);

TEST(FunctionFactoryTest, CreateFunctionTest)
{
  IFunction_sptr fun = FunctionFactory::instance().createFunction("FunctionFactoryTestFunction");
  EXPECT_TRUE( fun.get() != nullptr );
  //Function1DTestFunction fun;
  FunctionDomain1DVector domain(0.0,1.0,10);
  FunctionValues values(domain);
  fun->function(domain,values);
  for(size_t i = 0; i < domain.size(); ++i)
  {
    double x = domain[i];
    EXPECT_EQ( 1.0 * x - 3.0, values.getCalculated(i));
  }
}

TEST(FunctionFactoryTest, CreateFitFunctionTest)
{
  IFunction_sptr fun = FunctionFactory::instance().createFitFunction("FunctionFactoryTestFunction(A=10,B=20)");
  EXPECT_TRUE( fun.get() != nullptr );
  EXPECT_EQ(fun->getParameter("A"),10);
  EXPECT_EQ(fun->getParameter("B"),20);
}

TEST(FunctionFactoryTest, CreateCompositeFunctionTest)
{
  std::string ini = "CompositeFunction("
    "FunctionFactoryTestFunction(A=10,B=20),"
    "FunctionFactoryTestFunction(A=30,B=40)"
    ")";
  IFunction_sptr fun = FunctionFactory::instance().createFitFunction(ini);
  EXPECT_TRUE( fun.get() != nullptr );
  CompositeFunction_sptr cf = boost::dynamic_pointer_cast<CompositeFunction>(fun);
  EXPECT_TRUE( cf.get() != nullptr );
  EXPECT_EQ( cf->nFunctions(), 2 );
  auto f1 = cf->getFunction(0);
  auto f2 = cf->getFunction(1);
  EXPECT_EQ(f1->getParameter("A"),10);
  EXPECT_EQ(f1->getParameter("B"),20);
  EXPECT_EQ(f2->getParameter("A"),30);
  EXPECT_EQ(f2->getParameter("B"),40);
  EXPECT_EQ(cf->getParameter("f0.A"),10);
  EXPECT_EQ(cf->getParameter("f0.B"),20);
  EXPECT_EQ(cf->getParameter("f1.A"),30);
  EXPECT_EQ(cf->getParameter("f1.B"),40);
}
