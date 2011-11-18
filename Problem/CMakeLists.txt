#
#  Problem project
#
project(Problem)

set( INC_FILES 
   inc/Problem/DllExport.h
   inc/Problem/Problem.h
   )
set( SRC_FILES 
  src/Problem.cpp
  )
add_library( Problem SHARED ${SRC_FILES} ${INC_FILES} )
include_directories(inc ${THIRD_PARTY_INCLUDE_DIR})
set_target_properties ( Problem PROPERTIES COMPILE_DEFINITIONS IN_PROBLEM )

#
#  Problem Test project
#
project(ProblemTest)

#set( INC_FILES test/DummyVar.h )
set( SRC_FILES 
  test/main.cpp
  test/ProblemTest.cpp
)

add_executable(ProblemTest ${SRC_FILES} ${INC_FILES})
include_directories(test ${THIRD_PARTY_INCLUDE_DIR} )
find_library(GTEST_DEBUG_LIB gtest_d "${THIRD_PARTY_LIB_DIR}")
find_library(GTEST_RELEASE_LIB gtest "${THIRD_PARTY_LIB_DIR}")
target_link_libraries(ProblemTest 
${GTEST_LIBS} Formula Problem
)

