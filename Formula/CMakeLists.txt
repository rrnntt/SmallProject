#
#  Formula project
#
project(Formula)

set( INC_FILES 
   inc/Formula/DllExport.h
   inc/Formula/Bool.h
   inc/Formula/EParser.h
   inc/Formula/Expression.h
   inc/Formula/Function.h
   inc/Formula/FunctionFactory.h
   inc/Formula/MemberFunction.h
   inc/Formula/Namespace.h
   inc/Formula/Object.h
   inc/Formula/Operators.h
   inc/Formula/Parser.h
   inc/Formula/Reference.h
   inc/Formula/Scalar.h
   inc/Formula/ScriptLine.h
   inc/Formula/ScriptModule.h
   inc/Formula/ScriptModuleFunction.h
   inc/Formula/Str.h
   inc/Formula/Variable.h
   inc/Formula/VariableFactory.h
   )
set( SRC_FILES 
  src/Bool.cpp
  src/EParser.cpp
  src/Expression.cpp
  src/Function.cpp
  src/FunctionFactory.cpp
  src/Namespace.cpp
  src/Operators.cpp
  src/Parser.cpp
  src/Reference.cpp
  src/Scalar.cpp
  src/ScriptLine.cpp
  src/ScriptModule.cpp
  src/ScriptModuleFunction.cpp
  src/Str.cpp
  src/Variable.cpp
  src/VariableFactory.cpp
  )
add_library( Formula SHARED ${SRC_FILES} ${INC_FILES} )
include_directories(inc ${THIRD_PARTY_INCLUDE_DIR})
set_target_properties ( Formula PROPERTIES COMPILE_DEFINITIONS IN_FORMULA )

#
#  Formula Test project
#
project(FormulaTest)

set( INC_FILES test/DummyVar.h )
set( SRC_FILES 
  test/main.cpp
  test/BoolTest.cpp
  test/DummyVarTest.cpp
  test/EParserTest.cpp
  test/ParserTest.cpp
  test/VariableTest.cpp
  test/VariableFactoryTest.cpp
)

add_executable(FormulaTest ${SRC_FILES} ${INC_FILES})
include_directories(test ${THIRD_PARTY_INCLUDE_DIR} )
find_library(GTEST_DEBUG_LIB gtest_d "${THIRD_PARTY_LIB_DIR}")
find_library(GTEST_RELEASE_LIB gtest "${THIRD_PARTY_LIB_DIR}")
target_link_libraries(FormulaTest 
${GTEST_LIBS} Formula
)

