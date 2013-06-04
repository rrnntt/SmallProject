#include "gtest/gtest.h"
#include "Numeric/Schrodinger1D.h"
#include "Numeric/FunctionFactory.h"
#include "Numeric/IFunction.h"
#include "Numeric/ChebfunVector.h"
#include "Numeric/ChebOperator.h"
#include "API/TableWorkspace.h"

using namespace Numeric;
using namespace API;

TEST(Numeric_Schrodinger1D_Test, EigenTest)
{
  double beta = 0.5;
  double startX = -10.0;
  double endX = 10.0;

  auto VPot = FunctionFactory::instance().createFitFunction("UserFunction1D");
  VPot->setAttributeValue("Formula","x^2/2");

  Schrodinger1D alg;
  alg.setProperty("Beta", beta);
  alg.setProperty("StartX", startX);
  alg.setProperty("EndX", endX);
  alg.setClassProperty("VPot",VPot);
  alg.execute();

  ChebfunVector_sptr vec = alg.getClass("Eigenvectors");
  TableWorkspace_ptr e = alg.getClass("Eigenvalues");
  auto ener = e->getDoubleData("Energy");
  for(size_t i = 0; i < 10; ++i)
  {
      //std::cerr << i << ' ' << vec->cfun(i).cfun(0).n() << ' ' << ener[i] << std::endl;
      EXPECT_NEAR( ener[i], 0.5 + double(i), 0.01 );
      for(size_t j = i; j < 10; ++j)
      {
          ChebFunction tmp(vec->cfun(i));
          tmp *= vec->cfun(j);
          double integr = tmp.integr();
          //std::cerr << "integr("<<i<<","<<j<<") = " << integr << std::endl;
          if ( i == j )
          {
              EXPECT_NEAR(integr, 1.0, 1e-10 );
          }
          else
          {
              EXPECT_NEAR(integr, 0.0, 1e-7 );
          }
      }
  }
}

TEST(Numeric_Schrodinger1D_Test, HamiltonianTest)
{
    auto VPot = FunctionFactory::instance().createFitFunction("UserFunction1D");
    VPot->setAttributeValue("Formula","x^2/2");

    double beta = 0.5;
    double startX = -10.0;
    double endX = 10.0;

    auto kinet = new ChebCompositeOperator;
    kinet->addRight( new ChebTimes(-beta) );
    kinet->addRight( new ChebDiff2 );
    auto pot = new ChebTimes(VPot);
    auto hamiltonian = new ChebPlus;
    hamiltonian->add('+', kinet );
    hamiltonian->add('+', pot );

    chebfun vpot( 0, startX, endX );
    vpot.bestFit( *VPot );
    vpot.resize( 200 );

    GSLMatrix L;
    hamiltonian->createMatrix( vpot.getBase(), L );
    //kinet->createMatrix( vpot.getBase(), L );
    //pot->createMatrix( vpot.getBase(), L );

    //std::cerr << L << std::endl;
//    for(size_t i = 0; i < L.size2(); ++i)
//    {
//        std::cerr << i << ' ' << L.get(100,i) << std::endl;
//    }

    Schrodinger1D alg;
    alg.setProperty("Beta", beta);
    alg.setProperty("StartX", startX);
    alg.setProperty("EndX", endX);
    alg.setClassProperty("VPot",VPot);
    alg.execute();

    ChebfunVector_sptr vec = alg.getClass("Eigenvectors");
    TableWorkspace_ptr e = alg.getClass("Eigenvalues");
    auto ener = e->getDoubleData("Energy");

    size_t n = 30;
    GSLMatrix H(n,n);

    for(size_t i = 0; i < n; ++i)
    {
        const chebfun &f1 = vec->cfun(i).cfun(0).getUnscaled();
        chebfun d1(f1);
        hamiltonian->apply(f1, d1);
        chebfun dd = d1;
        dd *= f1;
        double me = dd.integr();
        H.set( i, i, me );
        //std::cerr << "e=" << me << std::endl;
        for(size_t j = i+1; j < n; ++j)
        {
            const chebfun &f2 = vec->cfun(j).cfun(0).getUnscaled();
            chebfun d2(f2);
            hamiltonian->apply(f2, d2);
            d2 *= f1;
            double me = d2.integr();
            //std::cerr << "o=" << me << std::endl;
            H.set( i, j, me );
            H.set( j, i, me );
        }
    }
    std::cerr << H << std::endl;
    GSLVector en;
    GSLMatrix v;
    H.diag(en,v);
    std::cerr << en << std::endl;
}
