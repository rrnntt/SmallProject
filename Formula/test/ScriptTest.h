#ifndef SCRIPTTEST_H
#define SCRIPTTEST_H

#include "../ScriptLine.h"
#include "../ScriptModule.h"
#include "../Namespace.h"
#include "../Scalar.h"
#include "TestSuite.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>

using namespace Formula;

class ScriptTest
{
public:
  static void test()
  {
    ScriptTest p;
//    p.testModule();
//    p.testLoadFromFile();
//    p.testRef();
    p.testMemberFunction();
  }

  void testModule()
  {
    Namespace_ptr ns(new Namespace());
    ScriptModule mod(ns,"module");
    ns->addVariable("Scalar","z");
    ns->addVariable("Scalar","w");
    ScriptFunction_ptr fun = mod.defFunction("first(Scalar:a)");
    fun->addLine("Scalar: x = 1");
    fun->addLine("Scalar: y = 2");
    fun->addLine("z = a*(x + y)");

    fun->setArgument("a","10");
    fun->eval();
    double z = mod.getNamespace()->getVariable("z")->as<Scalar>();
    mod.getNamespace()->childNamespace("first")->logVars();
    TS_ASSERT_EQUALS(mod.getNamespace()->childNamespace("first")->getSetVarNames().size(),3);
    TS_ASSERT_EQUALS(z,30);

    ScriptFunction_ptr fun1 = mod.defFunction("second(Scalar:a)");
    fun1->addLine("Scalar: x = 5");
    fun1->addLine("first(a = x)");
    fun1->eval();
    z = mod.getNamespace()->getVariable("z")->as<Scalar>();
    TS_ASSERT_EQUALS(z,15);

    ScriptFunction_ptr fun2 = mod.defFunction("third(Scalar:a)");
    fun2->addLine("first(a = 3)");
    fun2->eval();
    z = mod.getNamespace()->getVariable("z")->as<Scalar>();
    TS_ASSERT_EQUALS(z,9);

    ScriptFunction_ptr fun3 = mod.defFunction("fourth(Scalar:a)");
    fun3->addLine("Scalar: x = 1.5");
    fun3->addLine("first(a = 2*sin(x))");
    fun3->eval();
    z = mod.getNamespace()->getVariable("z")->as<Scalar>();
    TS_ASSERT_DELTA(z,5.985,0.0002);

    ScriptFunction_ptr fun4 = mod.defFunction("fifth(Scalar:a)");
    fun4->addLine("Scalar: x = 1.5");
    fun4->addLine("if(x < 1:z = 10,(x = 5; z=x+ 1))");
    fun4->addLine("if(x!=5:w=3,w=5)");
    fun4->eval();
    z = mod.getNamespace()->getVariable("z")->as<Scalar>();
    double w = mod.getNamespace()->getVariable("w")->as<Scalar>();
    TS_ASSERT_DELTA(z,6,0.0002);
    TS_ASSERT_DELTA(w,5,0.0002);

    ScriptFunction_ptr fun5 = mod.defFunction("sixth(Scalar:a)");
    fun5->addLine("Scalar: x = 0");
    fun5->addLine("for(Scalar: i=0,i<10,i=i+1,x = x + i)");
    fun5->addLine("z=x");
    fun5->eval();
    //fun5->eval();
    z = mod.getNamespace()->getVariable("z")->as<Scalar>();
    TS_ASSERT_DELTA(z,45,0.0002);
  }

  void testLoadFromFile()
  {
    Namespace_ptr ns(new Namespace());
    ScriptModule mod(ns,"module");
    ns->addVariable("Scalar","z");
    ns->addVariable("Scalar","w");
    ns->addVariable("Scalar","u");
    std::ofstream ofil("ScriptTest1.xml");
    ofil << "<?xml version=\"1.0\"?>\n";
    ofil <<
        "<module>\n"

        "<function def=\"plus(Scalar: a,Scalar: b)\" >\n"
        "z = a + b\n"
        "</function>\n"

        "<function def=\"noparams\" >\n"
        "w = sin(1)\n"
        "</function>\n"

        "<function def=\"main\" >\n"
        "Scalar: x = 1;\n"
        "Scalar: y = 2;\n"
        "plus(a=x,b=y);\n"
        "noparams\n"
        "</function>\n"

        "<function def=\"Scalar: times(Scalar: a,Scalar: b)\" >\n"
        "return = a * b\n"
        "</function>\n"

        "<function def=\"testTimes\" >\n"
        "Scalar: x = 3;\n"
        "Scalar: y = 2;\n"
        "u = times(x,y) + 1\n"
        "</function>\n"

        "</module>\n";
    ofil.close();

    mod.load("ScriptTest1.xml");
    //mod.getNamespace()->childNamespace("main")->logVars();
    mod.getFunction("main")->eval();
    double z = mod.getNamespace()->getVariable("z")->as<Scalar>();
    double w = mod.getNamespace()->getVariable("w")->as<Scalar>();
    //std::cerr<<"w="<<w<<'\n';
    TS_ASSERT_EQUALS(z,3);
    TS_ASSERT_EQUALS(w,sin(1.));
    mod.getFunction("testTimes")->eval();
    double u = mod.getNamespace()->getVariable("u")->as<Scalar>();
    //std::cerr<<"u="<<u<<'\n';
    TS_ASSERT_EQUALS(u,7);
  }

  void testRef()
  {
    Namespace_ptr ns(new Namespace());
    ScriptModule mod(ns,"module");
    ns->addVariable("Scalar","z");
    std::ofstream ofil("ScriptTest2.xml");
    ofil << "<?xml version=\"1.0\"?>\n";
    ofil <<
        "<module>\n"

        "<function def=\"main\" >\n"
        "Scalar: x = 1;\n"
        "Scalar: y = 2;\n"
        "Ref: r = y;\n"  // setting reference to y
        "r = x;\n"       // assigning value to variable referenced by r (ie y = x)
        "z = y"          // z == 1
        "</function>\n"

        "</module>\n";
    ofil.close();

    mod.load("ScriptTest2.xml");

    mod.getFunction("main")->eval();
    double z = mod.getNamespace()->getVariable("z")->as<Scalar>();
    std::cerr<<"z="<<z<<'\n';
    TS_ASSERT_EQUALS(z,1);
  }

  class MethodOwner
  {
  public:
    void doit(std::vector<Variable_ptr>&,Variable_ptr)
    {
      std::cerr << "Hello!" << std::endl;
    }

    void getScalar(std::vector<Variable_ptr>& args,Variable_ptr ret)
    {
      double x = args[0]->as<Scalar>();
      Scalar& s = ret->as<Scalar>();
      s.setValue(3.14*x);
    }
  };

  void testMemberFunction()
  {
    Namespace_ptr ns(new Namespace());
    ScriptModule mod(ns,"module");
    ns->addVariable("Scalar","z");
    MethodOwner mo;
    mod.addMemberFunction<MethodOwner>(&mo,&MethodOwner::doit,"doit");
    mod.addMemberFunction(&mo,&MethodOwner::getScalar,"getScalar","Scalar: x","Scalar");

    std::ofstream ofil("ScriptTest_MemberFunction.xml");
    ofil << "<?xml version=\"1.0\"?>\n";
    ofil <<
        "<module>\n"

        "<function def=\"main\" >\n"
        "z=getScalar(2);"
        "doit\n"
        "</function>\n"

        "</module>\n";
    ofil.close();

    mod.load("ScriptTest_MemberFunction.xml");
    //mod.logFuns();

    mod.getFunction("main")->eval();

    double z = mod.getNamespace()->getVariable("z")->as<Scalar>();
    std::cerr<<"z="<<z<<'\n';
    TS_ASSERT_EQUALS(z,6.28);
  }

};
#endif // SCRIPTTEST_H
