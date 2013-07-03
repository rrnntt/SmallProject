#include "gtest/gtest.h"
#include "Numeric/FunctionDomainMD.h"

using namespace Numeric;

TEST(Numeric_FunctionDomainMD_Test, Test)
{
    FunctionDomainMDGrid grid(3);
    EXPECT_EQ( grid.ndim(), 3 );
    EXPECT_EQ( grid.size(), 1 );

    std::vector<double> axis1(3);
    axis1[0] = 1;
    axis1[1] = 2;
    axis1[2] = 3;

    std::vector<double> axis2(2);
    axis2[0] = 4;
    axis2[1] = 5;

    std::vector<double> axis3(2);
    axis3[0] = 6;
    axis3[1] = 7;

    grid.setDimension(0, axis1);
    grid.setDimension(1, axis2);
    grid.setDimension(2, axis3);
    EXPECT_EQ( grid.size(), 3*2*2 );

    EXPECT_THROW(grid.setDimension(3, axis3), std::runtime_error);
    EXPECT_THROW(grid.setDimension(10, axis3), std::runtime_error);

    const double* point = grid.getPoint(2);
    EXPECT_EQ( point[0], 3 );
    EXPECT_EQ( point[1], 4 );
    EXPECT_EQ( point[2], 6 );

    point = grid.getPoint(4);
    EXPECT_EQ( point[0], 2 );
    EXPECT_EQ( point[1], 5 );
    EXPECT_EQ( point[2], 6 );

    point = grid.getPoint(9);
    EXPECT_EQ( point[0], 1 );
    EXPECT_EQ( point[1], 5 );
    EXPECT_EQ( point[2], 7 );
}
