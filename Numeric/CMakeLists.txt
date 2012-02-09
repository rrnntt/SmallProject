#
#  Numeric project
#
#project(Numeric)

set( INC_FILES 
   inc/Numeric/Chebfun.h
   inc/Numeric/ConjugateFRMinimizer.h
   inc/Numeric/CostFunction.h
   inc/Numeric/DllExport.h
   inc/Numeric/Distribution.h
   inc/Numeric/FunctionDomain.h
   inc/Numeric/FunctionDomain1D.h
   inc/Numeric/Function1D.h
   inc/Numeric/IFunction.h
   inc/Numeric/Jacobian.h
   inc/Numeric/LeastSquares.h
   inc/Numeric/Minimizer.h
   inc/Numeric/ParamFunction.h
   inc/Numeric/SimplexMinimizer.h
   inc/Numeric/SeriesFunction.h
   inc/Numeric/SomeFunction.h
   inc/Numeric/Vector.h
   inc/Numeric/V3D.h
   inc/Numeric/PhysModel.h
   inc/Numeric/PhysBody.h
   )
set( SRC_FILES 
  src/Chebfun.cpp
  src/ConjugateFRMinimizer.cpp
  src/CostFunction.cpp
  src/Distribution.cpp
  src/FunctionDomain.cpp
  src/FunctionDomain1D.cpp
  src/Function1D.cpp
  src/IFunction.cpp
  src/LeastSquares.cpp
  src/Minimizer.cpp
  src/ParamFunction.cpp
  src/SimplexMinimizer.cpp
  src/SeriesFunction.cpp
  src/SomeFunction.cpp
  src/Vector.cpp
  src/V3D.cpp
  src/PhysModel.cpp
  src/PhysBody.cpp
  )
add_library( Numeric SHARED ${SRC_FILES} ${INC_FILES} )
include_directories(inc ${THIRD_PARTY_INCLUDE_DIR})
set_target_properties ( Numeric PROPERTIES COMPILE_DEFINITIONS IN_NUMERIC )

set( GSL_DEBUG_LIB "${THIRD_PARTY_LIB_DIR}/gsl_d.lib")
set( GSL_RELEASE_LIB "${THIRD_PARTY_LIB_DIR}/gsl.lib")
set( GSL_LIBS "optimized;${GSL_RELEASE_LIB};debug;${GSL_DEBUG_LIB};")


target_link_libraries(Numeric
${GSL_LIBS}
)

#
#  Numeric Test project
#
#project(NumericTest)

set( INC_FILES )
set( SRC_FILES 
  test/main.cpp
  test/ChebfunTest.cpp
  test/ConjugateFRMinimizerTest.cpp
  test/DistributionTest.cpp
  test/LeastSquaresTest.cpp
  test/Function1DTest.cpp
  test/ParamFunctionTest.cpp
  test/SeriesFunctionTest.cpp
  test/SimplexMinimizerTest.cpp
  test/SomeFunctionTest.cpp
  test/VectorTest.cpp
)

add_executable(NumericTest ${SRC_FILES} ${INC_FILES})
include_directories(test ${THIRD_PARTY_INCLUDE_DIR} )
find_library(GTEST_DEBUG_LIB gtest_d "${THIRD_PARTY_LIB_DIR}")
find_library(GTEST_RELEASE_LIB gtest "${THIRD_PARTY_LIB_DIR}")
target_link_libraries(NumericTest 
${GTEST_LIBS} ${GSL_LIBS} Numeric
)

# Add to UnitTests folder in an IDE
set_property ( TARGET NumericTest PROPERTY FOLDER "UnitTests" )
