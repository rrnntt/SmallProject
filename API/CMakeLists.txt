#
#  API project
#
project(API)

set( INC_FILES 
   inc/API/DllExport.h
   inc/API/Framework.h
   inc/API/Singleton.h
   inc/API/Workspace.h
   inc/API/WorkspaceFactory.h
   )
set( SRC_FILES 
   src/Framework.cpp
   src/Workspace.cpp
   src/WorkspaceFactory.cpp
  )
add_library( API SHARED ${SRC_FILES} ${INC_FILES} )
include_directories(inc ${THIRD_PARTY_INCLUDE_DIR})
set_target_properties ( API PROPERTIES COMPILE_DEFINITIONS IN_API )

#
#  API Test project
#
project(APITest)

set( SRC_FILES 
  test/main.cpp
)

add_executable(APITest ${SRC_FILES})
include_directories(test ${THIRD_PARTY_INCLUDE_DIR} )
find_library(GTEST_DEBUG_LIB gtest_d "${THIRD_PARTY_LIB_DIR}")
find_library(GTEST_RELEASE_LIB gtest "${THIRD_PARTY_LIB_DIR}")
target_link_libraries(APITest 
${GTEST_LIBS} API
)

