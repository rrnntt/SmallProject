#
#  Numeric project
#
#project(Numeric)

set( INC_FILES 
   inc/Numeric/BFGS_Minimizer.h
   inc/Numeric/Chebfun.h
   inc/Numeric/Chebfun2DSpline.h
   inc/Numeric/ChebfunVector.h
   inc/Numeric/ChebFunction.h
   inc/Numeric/ChebOperator.h
   inc/Numeric/Chebyshev.h
   inc/Numeric/CompositeDomain.h
   inc/Numeric/CompositeFunction.h
   inc/Numeric/ConstraintFactory.h
   inc/Numeric/CostFunctionFactory.h
   inc/Numeric/CostFuncFitting.h
   inc/Numeric/Constants.h
   inc/Numeric/CustomPolynomial.h
   inc/Numeric/DllExport.h
   inc/Numeric/Distribution.h
   inc/Numeric/DerivMinimizer.h
   inc/Numeric/DomainMap.h
   inc/Numeric/FuncMinimizerFactory.h
   inc/Numeric/FunctionDomain.h
   inc/Numeric/FunctionDomain1D.h
   inc/Numeric/FunctionFactory.h
   inc/Numeric/FunctionValues.h
   inc/Numeric/GSLJacobian.h
   inc/Numeric/GSLMatrix.h
   inc/Numeric/GSLVector.h
   inc/Numeric/IConstraint.h
   inc/Numeric/ICostFunction.h
   inc/Numeric/IFuncMinimizer.h
   inc/Numeric/IFunction.h
   inc/Numeric/IFunction1D.h
   inc/Numeric/IFunctionValues.h
   inc/Numeric/IPeakFunction.h
   inc/Numeric/Jacobian.h
   inc/Numeric/JacobiPolynomial.h
   inc/Numeric/JacobiWeight.h
   inc/Numeric/JointDomain.h
   inc/Numeric/JointDomainFunction.h
   inc/Numeric/Laguerre.h
   inc/Numeric/LaguerreWeight.h
   inc/Numeric/LeastSquares.h
   inc/Numeric/LevenbergMarquardt.h
   inc/Numeric/MultiDomainFunction.h
   inc/Numeric/ParameterReference.h
   inc/Numeric/ParameterTie.h
   inc/Numeric/ParamFunction.h
   inc/Numeric/PhysBody.h
   inc/Numeric/PhysModel.h
   inc/Numeric/Polynomial.h
   inc/Numeric/ProductFunction.h
   inc/Numeric/Quadrature.h
#   inc/Numeric/SeriesFunction.h
   inc/Numeric/ScaledChebfun.h
   inc/Numeric/SchrodScale.h
   inc/Numeric/Schrodinger0.h
   inc/Numeric/Schrodinger.h
   inc/Numeric/SolveODE.h
   inc/Numeric/StdJacobian.h
   inc/Numeric/UserFunction1D.h
   inc/Numeric/V3D.h
   inc/Numeric/Vector.h
# Algorithms
   inc/Numeric/AddChebfun.h
   inc/Numeric/AddFunctionValuesToTable.h
   inc/Numeric/AnalyseBasis.h
   inc/Numeric/ChebfunWorkspace.h
   inc/Numeric/CopyChebfun.h
   inc/Numeric/CreateChebfun.h
   inc/Numeric/CreateQuadratureChebfuns.h
   inc/Numeric/ChebfunToTable.h
   inc/Numeric/DiffChebfun.h
   inc/Numeric/EvalChebfun.h
   inc/Numeric/Fit.h
   inc/Numeric/MakeQuadratureScheme.h
   inc/Numeric/MakeQuadrature.h
   inc/Numeric/UniformChebfun.h

   )
set( SRC_FILES 
  src/BFGS_Minimizer.cpp
  src/Chebfun.cpp
  src/Chebfun2DSpline.cpp
  src/ChebfunVector.cpp
  src/ChebFunction.cpp
  src/ChebOperator.cpp
  src/Chebyshev.cpp
  src/CompositeFunction.cpp
  src/ConstraintFactory.cpp
  src/CostFunctionFactory.cpp
  src/CostFuncFitting.cpp
  src/Constants.cpp
  src/CustomPolynomial.cpp
  src/Distribution.cpp
  src/DerivMinimizer.cpp
  src/DomainMap.cpp
  src/FuncMinimizerFactory.cpp
  src/FunctionDomain.cpp
  src/FunctionDomain1D.cpp
  src/FunctionFactory.cpp
  src/FunctionValues.cpp
  src/IFuncMinimizer.cpp
  src/IFunction.cpp
  src/IFunction1D.cpp
  src/IPeakFunction.cpp
  src/JacobiPolynomial.cpp
  src/JacobiWeight.cpp
  src/JointDomain.cpp
  src/JointDomainFunction.cpp
  src/Laguerre.cpp
  src/LaguerreWeight.cpp
  src/LeastSquares.cpp
  src/LevenbergMarquardt.cpp
  src/MultiDomainFunction.cpp
  src/ParamFunction.cpp
  src/ParameterReference.cpp
  src/ParameterTie.cpp
  src/Polynomial.cpp
  src/ProductFunction.cpp
  src/Quadrature.cpp
  src/ScaledChebfun.cpp
#  src/SeriesFunction.cpp
  src/SchrodScale.cpp
  src/Schrodinger0.cpp
  src/Schrodinger.cpp
  src/SolveODE.cpp
  src/UserFunction1D.cpp
  src/Vector.cpp
  src/V3D.cpp
  src/PhysModel.cpp
  src/PhysBody.cpp
# Algorithms
   src/AddChebfun.cpp
   src/AddFunctionValuesToTable.cpp
   src/AnalyseBasis.cpp
   src/ChebfunWorkspace.cpp
   src/CopyChebfun.cpp
   src/CreateChebfun.cpp
   src/CreateQuadratureChebfuns.cpp
   src/ChebfunToTable.cpp
   src/DiffChebfun.cpp
   src/EvalChebfun.cpp
   src/Fit.cpp
   src/MakeQuadratureScheme.cpp
   src/MakeQuadrature.cpp
   src/UniformChebfun.cpp
  )
add_library( Numeric SHARED ${SRC_FILES} ${INC_FILES} )
include_directories(inc ../Kernel/inc ../API/inc ../Formula/inc ${THIRD_PARTY_INCLUDE_DIR})
set_target_properties ( Numeric PROPERTIES COMPILE_DEFINITIONS IN_NUMERIC )

if (CMAKE_HOST_WIN32)
  set( GSL_DEBUG_LIB "${THIRD_PARTY_LIB_DIR}/gsl_d.lib")
  set( GSL_RELEASE_LIB "${THIRD_PARTY_LIB_DIR}/gsl.lib")
  set( GSL_LIBS "optimized;${GSL_RELEASE_LIB};debug;${GSL_DEBUG_LIB};")
else()
  find_package( GSL )
endif()

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
  test/ChebFunctionTest.cpp
  test/ChebOperatorTest.cpp
  test/ChebfunTest.cpp
  test/ChebfunVectorTest.cpp
  test/Chebfun2DSplineTest.cpp
  #test/ConjugateFRMinimizerTest.cpp
  test/CostFuncFittingTest.cpp
  test/CustomPolynomialTest.cpp
  test/DistributionTest.cpp
  #test/LeastSquaresTest.cpp
  test/Function1DTest.cpp
  test/FunctionDomain1DTest.cpp
  test/FunctionFactoryTest.cpp
  test/FuncMinimizerFactoryTest.cpp
  test/JacobiPolynomialTest.cpp
  test/JacobiWeightTest.cpp
  test/JointDomainFunctionTest.cpp
  test/GSLMatrixTest.cpp
  test/IFuncMinimizerTest.cpp
  test/LaguerreTest.cpp
  test/LaguerreWeightTest.cpp
  test/LeastSquaresTest.cpp
  test/LevenbergMarquardtTest.cpp
  test/ParamFunctionTest.cpp
  test/ProductFunctionTest.cpp
  test/QuadratureTest.cpp
  test/ScaledChebfunTest.cpp
  test/SchrodScaleTest.cpp
  test/Schrodinger0Test.cpp
  test/SeriesFunctionTest.cpp
  #test/SimplexMinimizerTest.cpp
  test/SolveODETest.cpp
  test/UserFunction1DTest.cpp
  test/VectorTest.cpp
# Algorithms
  test/AddChebfunTest.cpp
  test/AddFunctionValuesToTableTest.cpp
  test/AnalyseBasisTest.cpp
  test/ChebfunWorkspaceTest.cpp
  test/CreateChebfunTest.cpp
  test/DiffChebfunTest.cpp
  test/FitTest.cpp
  test/MakeQuadratureSchemeTest.cpp
  test/MakeQuadratureTest.cpp
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
