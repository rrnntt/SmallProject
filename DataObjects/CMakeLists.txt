#
#  DataObjects project
#
project(DataObjects)

set( INC_FILES 
   inc/DataObjects/DllExport.h
   )
set( SRC_FILES 
  src/Bool.cpp
  )
add_library( DataObjects SHARED ${SRC_FILES} ${INC_FILES} )
include_directories(inc ${THIRD_PARTY_INCLUDE_DIR})
set_target_properties ( DataObjects PROPERTIES COMPILE_DEFINITIONS IN_DATAOBJECTS )

#
#  DataObjects Test project
#
project(DataObjectsTest)

set( SRC_FILES 
  test/main.cpp
)

add_executable(DataObjectsTest ${SRC_FILES})
include_directories(test ${THIRD_PARTY_INCLUDE_DIR} )
find_library(GTEST_DEBUG_LIB gtest_d "${THIRD_PARTY_LIB_DIR}")
find_library(GTEST_RELEASE_LIB gtest "${THIRD_PARTY_LIB_DIR}")
target_link_libraries(DataObjectsTest 
${GTEST_LIBS} DataObjects
)

