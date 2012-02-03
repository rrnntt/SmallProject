#include "gtest/gtest.h"
#include "Numeric/LeastSquares.h"
#include "Numeric/ConjugateFRMinimizer.h"
#include "Numeric/SimplexMinimizer.h"
#include "Numeric/IFunction.h"
#include "Numeric/FunctionDomain1D.h"

#include <iostream>

using namespace Numeric;

class LeastSquaresTestFunction: public IFunction
{
public:
  LeastSquaresTestFunction():A(0.1),B(0.1){}
  /// Returns the function's name
  virtual std::string name()const {return "LeastSquaresTestFunction";}

  /// Function you want to fit to. 
  /// @param domain :: The buffer for writing the calculated values. Must be big enough to accept dataSize() values
  virtual void function(FunctionDomain& domain)const
  {
    FunctionDomain1D* d1d = dynamic_cast<FunctionDomain1D*>(&domain);
    if (!d1d) return;
    for(size_t i = 0; i < domain.size(); ++i)
    {
      double x = d1d->getX(i);
      d1d->setCalculated(i, A*x+B);
    }
  }
  /// Derivatives of function with respect to active parameters
  virtual void functionDeriv(FunctionDomain& domain, Jacobian& jacobian)
  {
    FunctionDomain1D* d1d = dynamic_cast<FunctionDomain1D*>(&domain);
    if (!d1d) return;
    for(size_t i = 0; i < domain.size(); ++i)
    {
      double x = d1d->getX(i);
      jacobian.set(i,0,x);
      jacobian.set(i,1,1.0);
    }
  }

  /// Get i-th parameter
  virtual double getParameter(size_t i)const {return (i==0)?A:B;}
  /// Get parameter by name.
  virtual double getParameter(const std::string& name)const {return 0;}
  /// Set i-th parameter
  virtual void setParameter(size_t i, const double& value) {if (i==0) A=value;else B=value;}
  /// Set parameter by name.
  virtual void setParameter(const std::string& name, const double& value) {}
  /// Total number of parameters
  virtual size_t nParams()const {return 2;}
  /// Returns the index of parameter name
  virtual size_t parameterIndex(const std::string& name)const {return 0;}
  /// Returns the name of parameter i
  virtual std::string parameterName(size_t i)const {return (i==0)?"A":"B";}

  /// Number of active (in terms of fitting) parameters
  virtual size_t nActive()const {return 2;}
  /// Value of i-th active parameter. Override this method to make fitted parameters different from the declared
  virtual double activeParameter(size_t i)const {return (i==0)?A:B;}
  /// Set new value of i-th active parameter. Override this method to make fitted parameters different from the declared
  virtual void setActiveParameter(size_t i, double value) {if (i==0) A=value;else B=value;}
  /// Returns the name of active parameter i
  virtual std::string nameOfActive(size_t i)const {return (i==0)?"A":"B";}

  /// Check if declared parameter i is fixed
  virtual bool isFixed(size_t i)const {return false;}
  /// Fixes declared parameter i
  virtual void fix(size_t i) {}
  /// Restores declared parameter i for fitting
  virtual void unfix(size_t i) {}

protected:

  /// Declare a new parameter
  virtual void declareParameter(const std::string& name, double initValue = 0) {}

  double A,B;
};

TEST(LeastSquaresTest, Test)
{
  std::vector<double> xx(10),data(10);
  std::generate(xx.begin(),xx.end(),[]()->double{static double x = 0;
    x+=0.1;
    return x;
  });
  std::transform(xx.begin(),xx.end(),data.begin(),[](double x)->double{
    return 5*x - 3; 
  });

  FunctionDomain_ptr domain(new FunctionDomain1D(xx));
  domain->setFitData(data);
  LeastSquares* ls = new LeastSquares;
  ls->setFunction(IFunction_ptr(new LeastSquaresTestFunction));
  ls->setDomain(domain);
  CostFunction_ptr fun(ls);

  //SimplexMinimizer mzr;
  ConjugateFRMinimizer mzr;
  mzr.setCostFunction(fun);
  mzr.minimize();
  std::cerr << "LeastSquares " << fun->eval() << std::endl;
  std::cerr << "A=" << fun->getParameter(0) << std::endl;
  std::cerr << "B=" << fun->getParameter(1) << std::endl;
}

