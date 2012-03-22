#
#  DataObjects project
#
#project(DataObjects)

set( INC_FILES 
   inc/DataObjects/DllExport.h
   inc/DataObjects/ChebfunWorkspace.h
   inc/DataObjects/Column.h
   inc/DataObjects/ColumnVector.h
   inc/DataObjects/CompoundColumn.h
   inc/DataObjects/ColumnFactory.h
   inc/DataObjects/NumericColumn.h
   inc/DataObjects/StringColumn.h
   inc/DataObjects/TableColumn.h
   inc/DataObjects/TableWorkspace.h
   inc/DataObjects/Unit.h
# Algorithms
   inc/DataObjects/CalculateColumnValues.h
   inc/DataObjects/CreateChebfun.h
   inc/DataObjects/CreateUniformRandomData.h
   inc/DataObjects/CreateDistribution.h
   )
set( SRC_FILES 
   src/ChebfunWorkspace.cpp
   src/Column.cpp
   src/ColumnVector.cpp
   src/NumericColumn.cpp
   src/ColumnFactory.cpp
   src/StringColumn.cpp
   src/TableColumn.cpp
   src/TableWorkspace.cpp
   src/Unit.cpp
# Algorithms
   src/CalculateColumnValues.cpp
   src/CreateChebfun.cpp
   src/CreateUniformRandomData.cpp
   src/CreateDistribution.cpp
  )
add_library( DataObjects SHARED ${SRC_FILES} ${INC_FILES} )
include_directories(inc ../Kernel/inc ../API/inc ../Formula/inc ../Numeric/inc ${THIRD_PARTY_INCLUDE_DIR})
set_target_properties ( DataObjects PROPERTIES COMPILE_DEFINITIONS IN_DATAOBJECTS )
target_link_libraries(DataObjects Kernel API Formula Numeric)

#
#  DataObjects Test project
#
#project(DataObjectsTest)

set( SRC_FILES 
  test/main.cpp
  test/ChebfunWorkspaceTest.cpp
  test/CreateChebfunTest.cpp
  test/CalculateColumnValuesTest.cpp
  test/TableTest.cpp
)

add_executable(DataObjectsTest ${SRC_FILES})
include_directories(test ${THIRD_PARTY_INCLUDE_DIR} )
find_library(GTEST_DEBUG_LIB gtest_d "${THIRD_PARTY_LIB_DIR}")
find_library(GTEST_RELEASE_LIB gtest "${THIRD_PARTY_LIB_DIR}")
target_link_libraries(DataObjectsTest 
${GTEST_LIBS} DataObjects 
)
# Add to UnitTests folder in an IDE
set_property ( TARGET DataObjectsTest PROPERTY FOLDER "UnitTests" )

