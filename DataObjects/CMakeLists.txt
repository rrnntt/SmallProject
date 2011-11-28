#
#  DataObjects project
#
project(DataObjects)

set( INC_FILES 
   inc/DataObjects/DllExport.h
   inc/DataObjects/Column.h
   inc/DataObjects/NumericColumn.h
   inc/DataObjects/ColumnFactory.h
   inc/DataObjects/TableColumn.h
   inc/DataObjects/TableWorkspace.h
   inc/DataObjects/Unit.h
   )
set( SRC_FILES 
   src/Column.cpp
   src/NumericColumn.cpp
   src/ColumnFactory.cpp
   src/TableColumn.cpp
   src/TableWorkspace.cpp
   src/Unit.cpp
  )
add_library( DataObjects SHARED ${SRC_FILES} ${INC_FILES} )
include_directories(inc ../Kernel/inc ../API/inc ${THIRD_PARTY_INCLUDE_DIR})
set_target_properties ( DataObjects PROPERTIES COMPILE_DEFINITIONS IN_DATAOBJECTS )
target_link_libraries(DataObjects Kernel API)

#
#  DataObjects Test project
#
project(DataObjectsTest)

set( SRC_FILES 
  test/main.cpp
  test/TableTest.cpp
)

add_executable(DataObjectsTest ${SRC_FILES})
include_directories(test ${THIRD_PARTY_INCLUDE_DIR} )
find_library(GTEST_DEBUG_LIB gtest_d "${THIRD_PARTY_LIB_DIR}")
find_library(GTEST_RELEASE_LIB gtest "${THIRD_PARTY_LIB_DIR}")
target_link_libraries(DataObjectsTest 
${GTEST_LIBS} DataObjects
)

