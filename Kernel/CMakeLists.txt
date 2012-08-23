
#
#  Kernel project
#
#project(Kernel)

set( INC_FILES 
   inc/Kernel/Creator.h
   inc/Kernel/CommonProperties.h
   inc/Kernel/DataService.h
   inc/Kernel/DllExport.h
   inc/Kernel/DllOpen.h
   inc/Kernel/GeometryTypes.h
   inc/Kernel/LibraryManager.h
   inc/Kernel/LibraryWrapper.h
   inc/Kernel/PropertyClassFactory.h
   inc/Kernel/DynamicFactory.h
   inc/Kernel/Logger.h
   inc/Kernel/Notification.h
   inc/Kernel/EParser.h
   inc/Kernel/Operators.h
   inc/Kernel/ParsedExpressionString.h
   inc/Kernel/Property.h
   inc/Kernel/PropertyClass.h
   inc/Kernel/PropertyManager.h
   inc/Kernel/Str.h
   inc/Kernel/Singleton.h
   inc/Kernel/System.h
   inc/Kernel/Framework.h
   )
set( SRC_FILES 
  src/CommonProperties.cpp
  src/DataService.cpp
  src/Logger.cpp
  src/Notification.cpp
  src/DllOpen.cpp
  src/LibraryManager.cpp
  src/LibraryWrapper.cpp
  src/EParser.cpp
  src/Operators.cpp
  src/ParsedExpressionString.cpp
  src/Property.cpp
  src/PropertyClass.cpp
  src/PropertyManager.cpp
  src/Str.cpp
  src/Framework.cpp
  )
add_library( Kernel SHARED ${SRC_FILES} ${INC_FILES} )
include_directories(inc)
set_target_properties ( Kernel PROPERTIES COMPILE_DEFINITIONS IN_KERNEL )

#target_link_libraries(Numeric
#debug ${GSL_LIB_DEBUG}
#optimized ${GSL_LIB} Numeric
#)

#
#  Kernel Test project
#
#project(KernelTest)

set( INC_FILES )
set( SRC_FILES 
  test/main.cpp
  test/EParserTest.cpp
  test/GeometryTypesTest.cpp
  test/ParsedExpressionStringTest.cpp
  test/PropertyTest.cpp
  test/PropertyClassTest.cpp
  test/PropertyManagerTest.cpp
)

add_executable(KernelTest ${SRC_FILES} ${INC_FILES})
include_directories(test ${THIRD_PARTY_INCLUDE_DIR} )
find_library(GTEST_DEBUG_LIB gtest_d "${THIRD_PARTY_LIB_DIR}")
find_library(GTEST_RELEASE_LIB gtest "${THIRD_PARTY_LIB_DIR}")
target_link_libraries(KernelTest ${GTEST_LIBS} Kernel)

# Add to UnitTests folder in an IDE
set_property ( TARGET KernelTest PROPERTY FOLDER "UnitTests" )
