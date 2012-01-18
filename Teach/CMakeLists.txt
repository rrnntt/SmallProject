#
#  Teach project
#
#project(Teach)

set( INC_FILES 
   inc/Teach/DllExport.h
   inc/Teach/Problem.h
   inc/Teach/Text.h
   )
set( SRC_FILES 
  src/Problem.cpp
  src/Text.cpp
  )
add_library( Teach SHARED ${SRC_FILES} ${INC_FILES} )
include_directories(inc 
../Kernel/inc
../API/inc
../Formula/inc
${THIRD_PARTY_INCLUDE_DIR}
)
set_target_properties ( Teach PROPERTIES COMPILE_DEFINITIONS IN_TEACH )
target_link_libraries(Teach
Kernel API Formula 
)

#
#  TeachTest project
#
#project(TeachTest)

set( INC_FILES )
set( SRC_FILES 
  test/main.cpp
  test/ProblemTest.cpp
)

add_executable(TeachTest ${SRC_FILES} ${INC_FILES})
include_directories(test ${THIRD_PARTY_INCLUDE_DIR} )
find_library(GTEST_DEBUG_LIB gtest_d "${THIRD_PARTY_LIB_DIR}")
find_library(GTEST_RELEASE_LIB gtest "${THIRD_PARTY_LIB_DIR}")
target_link_libraries(TeachTest 
${GTEST_LIBS} Formula Teach
)

# Add to UnitTests folder in an IDE
set_property ( TARGET TeachTest PROPERTY FOLDER "UnitTests" )
