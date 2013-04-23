#
#  SmallProject
#

# Add the path to our custom 'find' modules
set ( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/Build/CMake")

project(SmallProject)

cmake_minimum_required (VERSION 2.8)

if (CMAKE_HOST_UNIX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++0x")
endif()

set( THIRD_PARTY_DIR "${PROJECT_SOURCE_DIR}/Third_Party" )
set( THIRD_PARTY_INCLUDE_DIR "${THIRD_PARTY_DIR}/include" )

if (MSVC)
	if (CMAKE_CL_64)
	set( THIRD_PARTY_LIB_DIR "${THIRD_PARTY_DIR}/lib/win64" )
	else ()
	set( THIRD_PARTY_LIB_DIR "${THIRD_PARTY_DIR}/lib/win32" )
	endif ()
endif ()

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin" )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug" )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release" )

set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin" )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Debug" )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Release" )

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin" )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Debug" )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Release" )

if (CMAKE_HOST_WIN32)
  set( GTEST_DEBUG_LIB "${THIRD_PARTY_LIB_DIR}/gtest_d.lib")
  set( GTEST_RELEASE_LIB "${THIRD_PARTY_LIB_DIR}/gtest.lib")
  set( GTEST_LIBS "optimized;${GTEST_RELEASE_LIB};debug;${GTEST_DEBUG_LIB};")
else()
  set( GTEST_LIBS "gtest")
endif()

set ( Boost_NO_BOOST_CMAKE TRUE )
find_package( Boost )
find_package( Qt4 )
add_definitions( -DBOOST_ALL_DYN_LINK -DQWT_DLL)
include(${QT_USE_FILE})

include_directories( ${Boost_INCLUDE_DIR} "${PROJECT_SOURCE_DIR}/Kernel/inc")
link_directories(${THIRD_PARTY_LIB_DIR})

# Allow organizing stuff into folders in IDE
set_property ( GLOBAL PROPERTY USE_FOLDERS ON )

set( QT_PROPERTY_BROWSER_INCLUDE "${PROJECT_SOURCE_DIR}/QtPropertyBrowser/src")
set( LIB_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} )

add_subdirectory(gtest-1.6.0)
add_subdirectory(Kernel)
add_subdirectory(API)
add_subdirectory(Numeric)
add_subdirectory(Formula)
add_subdirectory(DataObjects)
add_subdirectory(GUI)
add_subdirectory(qwt)
add_subdirectory(Main)
add_subdirectory(QtPropertyBrowser)
add_subdirectory(QtAPI)
add_subdirectory(Goblin)
