#
#  SmallProject
#

project(SmallProject)

cmake_minimum_required (VERSION 2.8)

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

#message( "\n${CMAKE_BINARY_DIR}\n\n" )

#file(COPY ${THIRD_PARTY_LIB_DIR}/gtest_d.dll 
#     DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG} )
#file(RENAME ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/gtest_d.dll 
#     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/gtest.dll )
#file(COPY ${THIRD_PARTY_LIB_DIR}/gtest.dll 
#     DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE} )
	 
set( GTEST_DEBUG_LIB "${THIRD_PARTY_LIB_DIR}/gtest_d.lib")
set( GTEST_RELEASE_LIB "${THIRD_PARTY_LIB_DIR}/gtest.lib")
set( GTEST_LIBS "optimized;${GTEST_RELEASE_LIB};debug;${GTEST_DEBUG_LIB};")

#set( Boost_INCLUDE_DIR "C:/Users/hqs74821/Work/mantid/Code/Third_Party/include")
find_package( Boost )
find_package( Qt4 )
find_path( OSG_DIR "include/osg/Referenced" )
add_definitions( -DBOOST_ALL_DYN_LINK -DQWT_DLL)
include(${QT_USE_FILE})

include_directories( ${Boost_INCLUDE_DIR} "${PROJECT_SOURCE_DIR}/Kernel/inc")
link_directories(${THIRD_PARTY_LIB_DIR})

# Allow organizing stuff into folders in IDE
set_property ( GLOBAL PROPERTY USE_FOLDERS ON )

add_subdirectory(Kernel)
add_subdirectory(API)
add_subdirectory(Numeric)
add_subdirectory(Formula)
add_subdirectory(Teach)
add_subdirectory(TeachQt)
add_subdirectory(DataObjects)
add_subdirectory(GUI)
if (EXISTS QT_QTWEBKIT_INCLUDE_DIR)
  add_subdirectory(WebGUI)
endif()
add_subdirectory(qwt)
add_subdirectory(Main)
add_subdirectory(Modeling)
add_subdirectory(QtAPI)
add_subdirectory(Goblin)
