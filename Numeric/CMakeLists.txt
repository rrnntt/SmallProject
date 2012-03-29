#
#  Numeric project
#
#project(Numeric)

set( INC_FILES 
   inc/Numeric/Chebfun.h
   inc/Numeric/CompositeDomain.h
   inc/Numeric/CompositeFunction.h
   inc/Numeric/ConstraintFactory.h
   inc/Numeric/DllExport.h
   inc/Numeric/Distribution.h
   inc/Numeric/FunctionDomain.h
   inc/Numeric/FunctionDomain1D.h
   inc/Numeric/FunctionFactory.h
   inc/Numeric/FunctionProperty.h
   inc/Numeric/FunctionValues.h
   inc/Numeric/IConstraint.h
   inc/Numeric/ICostFunction.h
   inc/Numeric/IFunction.h
   inc/Numeric/IFunction1D.h
   inc/Numeric/IPeakFunction.h
   inc/Numeric/Jacobian.h
   inc/Numeric/JointDomain.h
   inc/Numeric/MultiDomainFunction.h
   inc/Numeric/ParameterReference.h
   inc/Numeric/ParameterTie.h
   inc/Numeric/ParamFunction.h
   inc/Numeric/PhysBody.h
   inc/Numeric/PhysModel.h
#   inc/Numeric/SeriesFunction.h
   inc/Numeric/SomeFunction.h
   inc/Numeric/UserFunction1D.h
   inc/Numeric/V3D.h
   inc/Numeric/Vector.h
   )
set( SRC_FILES 
  src/Chebfun.cpp
  src/CompositeFunction.cpp
  src/ConstraintFactory.cpp
  src/CostFunctionFactory.cpp
  src/Distribution.cpp
  src/FunctionDomain.cpp
  src/FunctionDomain1D.cpp
  src/FunctionFactory.cpp
  src/FunctionProperty.cpp
  src/FunctionValues.cpp
  src/IFunction.cpp
  src/IFunction1D.cpp
  src/IPeakFunction.cpp
  src/JointDomain.cpp
  src/MultiDomainFunction.cpp
  src/ParamFunction.cpp
  src/ParameterReference.cpp
  src/ParameterTie.cpp
#  src/SeriesFunction.cpp
  src/SomeFunction.cpp
  src/UserFunction1D.cpp
  src/Vector.cpp
  src/V3D.cpp
  src/PhysModel.cpp
  src/PhysBody.cpp
  )
add_library( Numeric SHARED ${SRC_FILES} ${INC_FILES} )
include_directories(inc ../Kernel/inc ../API/inc ../Formula/inc ${THIRD_PARTY_INCLUDE_DIR})
set_target_properties ( Numeric PROPERTIES COMPILE_DEFINITIONS IN_NUMERIC )

set( GSL_DEBUG_LIB "${THIRD_PARTY_LIB_DIR}/gsl_d.lib")
set( GSL_RELEASE_LIB "${THIRD_PARTY_LIB_DIR}/gsl.lib")
set( GSL_LIBS "optimized;${GSL_RELEASE_LIB};debug;${GSL_DEBUG_LIB};")


target_link_libraries(Numeric Kernel API Formula
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
  test/FunctionPropertyTest.cpp
  test/ParamFunctionTest.cpp
  test/SeriesFunctionTest.cpp
  test/SimplexMinimizerTest.cpp
  test/SomeFunctionTest.cpp
  test/UserFunction1DTest.cpp
  test/VectorTest.cpp
)

add_executable(NumericTest ${SRC_FILES} ${INC_FILES})
include_directories(test ${THIRD_PARTY_INCLUDE_DIR} )
find_library(GTEST_DEBUG_LIB gtest_d "${THIRD_PARTY_LIB_DIR}")
find_library(GTEST_RELEASE_LIB gtest "${THIRD_PARTY_LIB_DIR}")
target_link_libraries(NumericTest 
${GTEST_LIBS} ${GSL_LIBS} Numeric Kernel
)

# Add to UnitTests folder in an IDE
set_property ( TARGET NumericTest PROPERTY FOLDER "UnitTests" )
