#
#  Teach project
#
#project(Teach)

set( INC_FILES 
   inc/Teach/DllExport.h
   inc/Teach/Graph.h
   inc/Teach/GraphShape.h
   inc/Teach/Problem.h
   inc/Teach/GeometryTask.h
   inc/Teach/Text.h
   )

set( SRC_FILES 
  src/GeometryTask.cpp
  src/Graph.cpp
  src/GraphShape.cpp
  src/Problem.cpp
  src/Text.cpp
  )

set( MOC_HEADERS
   inc/Teach/GeometryTask.h
   inc/Teach/Graph.h
)

set( UI_FILES
)

qt4_wrap_cpp(MOC_FILES ${MOC_HEADERS})
#qt4_wrap_ui(UI_HEADERS ${UI_FILES})

add_library( Teach SHARED ${SRC_FILES} ${INC_FILES} ${MOC_FILES} ${UI_HEADERS} )
include_directories(inc 
../Kernel/inc
../API/inc
../Formula/inc
../QtAPI/inc
 ../qwt/src
${THIRD_PARTY_INCLUDE_DIR}
)
set_target_properties ( Teach PROPERTIES COMPILE_DEFINITIONS IN_TEACH )
target_link_libraries(Teach
Kernel API Formula QtAPI qwt
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
