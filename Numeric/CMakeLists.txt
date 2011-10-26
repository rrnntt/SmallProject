#
#  Numeric project
#
project(Numeric)

set( INC_FILES 
   inc/nu/SomeFunction.h
   inc/nu/DllExport.h
   inc/nu/Vector.h
   inc/nu/V3D.h
   inc/nu/PhysModel.h
   inc/nu/PhysBody.h
   )
set( SRC_FILES 
  src/SomeFunction.cpp
  src/Vector.cpp
  src/V3D.cpp
  src/PhysModel.cpp
  src/PhysBody.cpp
  )
add_library( Numeric SHARED ${SRC_FILES} ${INC_FILES} )
include_directories(inc ${THIRD_PARTY_INCLUDE_DIR})
set_target_properties ( Numeric PROPERTIES COMPILE_DEFINITIONS IN_NUMERIC )

set( GSL_DEBUG_LIB "${THIRD_PARTY_LIB_DIR}/gsl_d.lib")
set( GSL_RELEASE_LIB "${THIRD_PARTY_LIB_DIR}/gsl.lib")
set( GSL_LIBS "optimized;${GSL_RELEASE_LIB};debug;${GSL_DEBUG_LIB};")


target_link_libraries(Numeric
${GSL_LIBS}
)

#
#  Numeric Test project
#
project(NumericTest)

set( INC_FILES )
set( SRC_FILES 
  test/main.cpp
  test/SomeFunctionTest.cpp
  test/VectorTest.cpp
)

add_executable(NumericTest ${SRC_FILES} ${INC_FILES})
include_directories(test ${THIRD_PARTY_INCLUDE_DIR} )
find_library(GTEST_DEBUG_LIB gtest_d "${THIRD_PARTY_LIB_DIR}")
find_library(GTEST_RELEASE_LIB gtest "${THIRD_PARTY_LIB_DIR}")
target_link_libraries(NumericTest 
${GTEST_LIBS} ${GSL_LIBS} Numeric
)

