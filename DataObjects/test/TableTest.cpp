#include "gtest/gtest.h"
#include "DataObjects/TableWorkspace.h"
#include "API/WorkspaceFactory.h"
#include "API/Framework.h"

#include <iostream>

using namespace DataObjects;

TEST(TableTest, LoadAscii) {
  std::string dir = API::Framework::instance().testDirectory() + "DataObjects/";

  TableWorkspace tws;
  tws.loadAscii(dir + "Table.txt");
  std::cerr << tws.rowCount() << 'x' << tws.columnCount() << std::endl;
  EXPECT_EQ(tws.rowCount(),30);
  EXPECT_EQ(tws.columnCount(),4);

  TableWorkspace tws1;
  tws1.loadAscii(dir + "Table_spaces.txt");
  std::cerr << tws1.rowCount() << 'x' << tws1.columnCount() << std::endl;
  EXPECT_EQ(tws1.rowCount(),30);
  EXPECT_EQ(tws1.columnCount(),4);
}

