#include "gtest/gtest.h"
#include "DataObjects/CalculateColumnValues.h"
#include "API/WorkspaceFactory.h"
#include "API/WorkspaceManager.h"
#include "API/TableWorkspace.h"

#include <iostream>

using namespace DataObjects;

TEST(CalculateColumnValuesTest, Test)
{
  API::TableWorkspace_ptr tws = API::WorkspaceFactory::instance().createWorkspace("TableWorkspace");
  tws->addDoubleColumn("X");
  tws->addDoubleColumn("Y");
  tws->setRowCount(10);

  CalculateColumnValues algo;
  algo.setProperty("Workspace",tws);

}

