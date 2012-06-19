#
#  API project
#
#project(API)

set( INC_FILES 
   inc/API/Algorithm.h
   inc/API/AlgorithmFactory.h
   inc/API/DllExport.h
   inc/API/PropertyFactory.h
   inc/API/Workspace.h
   inc/API/WorkspaceFactory.h
   inc/API/WorkspaceManager.h
   #inc/API/WorkspaceProperty.h
   inc/API/Column.h
   inc/API/ColumnVector.h
   inc/API/CompoundColumn.h
   inc/API/ColumnFactory.h
   inc/API/NumericColumn.h
   inc/API/StringColumn.h
   inc/API/TableColumn.h
   inc/API/TableWorkspace.h
   inc/API/Unit.h
   )
set( SRC_FILES 
   src/Algorithm.cpp
   src/AlgorithmFactory.cpp
   src/PropertyFactory.cpp
   src/Workspace.cpp
   src/WorkspaceFactory.cpp
   src/WorkspaceManager.cpp
   #src/WorkspaceProperty.cpp
   src/Column.cpp
   src/ColumnVector.cpp
   src/NumericColumn.cpp
   src/ColumnFactory.cpp
   src/StringColumn.cpp
   src/TableColumn.cpp
   src/TableWorkspace.cpp
   src/Unit.cpp
  )
add_library( API SHARED ${SRC_FILES} ${INC_FILES} )
include_directories(inc ${THIRD_PARTY_INCLUDE_DIR})
set_target_properties ( API PROPERTIES COMPILE_DEFINITIONS IN_API )
target_link_libraries(API Kernel)

#
#  API Test project
#
#project(APITest)

set( SRC_FILES 
  test/main.cpp
  test/AlgorithmTest.cpp
  test/TableTest.cpp
  #test/WorkspacePropertyTest.cpp
)

add_executable(APITest ${SRC_FILES})
include_directories(test ${THIRD_PARTY_INCLUDE_DIR} )
find_library(GTEST_DEBUG_LIB gtest_d "${THIRD_PARTY_LIB_DIR}")
find_library(GTEST_RELEASE_LIB gtest "${THIRD_PARTY_LIB_DIR}")
target_link_libraries(APITest 
${GTEST_LIBS} API
)
# Add to UnitTests folder in an IDE
set_property ( TARGET APITest PROPERTY FOLDER "UnitTests" )
