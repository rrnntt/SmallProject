#include "gtest/gtest.h"
#include "API/Algorithm.h"
#include "API/TableWorkspace.h"
#include "API/WorkspaceFactory.h"
#include "API/WorkspaceManager.h"
#include "Kernel/Framework.h"

#include <iostream>

using namespace API;

class TestAlgorithm: public Algorithm
{
public:
  TestAlgorithm()
  {
    this->declareInt("N", 10);
    this->declareWorkspace("DummyWS");
    this->declareWorkspace("WS");
  }
  virtual std::string name() const {return "TestAlgorithm";}
protected:
  virtual void exec()
  {
    int n = get("N");
    std::cerr << "N = " << n << std::endl;
    auto ws = WorkspaceFactory::instance().createWorkspace("TableWorkspace");
    setProperty("WS",ws);
  }
};



TEST(AlgorithmTest, WorkspaceProperties) 
{
  TestAlgorithm alg;
  alg.setProperty("DummyWS","test");
  alg.setProperty("WS","Stuff");
  alg.execute();

  Workspace_ptr dws = alg.getClass("DummyWS");
  std::cerr << dws->id() << ' ' << dws->name() << std::endl;

  Workspace_ptr ws = alg.getClass("WS");
  std::cerr << ws->id() << ' ' << ws->name() << std::endl;
}

