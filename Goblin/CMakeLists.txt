#
#  Goblin
#

set( INC_FILES 
   inc/Goblin/DllExport.h
   inc/Goblin/ComDiffDialog.h
   inc/Goblin/EnergyList.h
   inc/Goblin/GoblinTask.h
   inc/Goblin/LineList.h
   inc/Goblin/comdiff.h
   inc/Goblin/humlicek.h
   inc/Goblin/vjkg.h
   inc/Goblin/voigt.h
   )
set( SRC_FILES
  src/ComDiffDialog.cpp 
  src/EnergyList.cpp
  src/GoblinTask.cpp
  src/LineList.cpp
  src/comdiff.cpp
  src/humlicek.cpp
  src/vjkg.cpp
  src/voigt.cpp
  )

set( MOC_HEADERS
   inc/Goblin/GoblinTask.h
   inc/Goblin/ComDiffDialog.h
)

set( UI_FILES
   inc/Goblin/ComDiffDialog.ui
)

qt4_wrap_cpp(MOC_FILES ${MOC_HEADERS})
qt4_wrap_ui(UI_HEADERS ${UI_FILES})

add_library( Goblin SHARED ${SRC_FILES} ${INC_FILES} ${MOC_FILES} ${UI_HEADERS} )
include_directories(inc ${THIRD_PARTY_INCLUDE_DIR} ../API/inc ../DataObjects/inc ../QtAPI/inc
"${CMAKE_BINARY_DIR}/Goblin"
)
set_target_properties ( Goblin PROPERTIES COMPILE_DEFINITIONS IN_GOBLIN )

#set( GSL_DEBUG_LIB "${THIRD_PARTY_LIB_DIR}/gsl_d.lib")
#set( GSL_RELEASE_LIB "${THIRD_PARTY_LIB_DIR}/gsl.lib")
#set( GSL_LIBS "optimized;${GSL_RELEASE_LIB};debug;${GSL_DEBUG_LIB};")

target_link_libraries(Goblin Kernel DataObjects QtAPI
#${GSL_LIBS}
)

#
#  GoblinTest
#

set( INC_FILES )
set( SRC_FILES 
  test/main.cpp
  test/EnergyListTest.cpp
)

add_executable(GoblinTest ${SRC_FILES} ${INC_FILES})
include_directories(test ${THIRD_PARTY_INCLUDE_DIR} )
find_library(GTEST_DEBUG_LIB gtest_d "${THIRD_PARTY_LIB_DIR}")
find_library(GTEST_RELEASE_LIB gtest "${THIRD_PARTY_LIB_DIR}")
target_link_libraries(GoblinTest 
${GTEST_LIBS} ${GSL_LIBS} Goblin
)

# Add to UnitTests folder in an IDE
set_property ( TARGET GoblinTest PROPERTY FOLDER "UnitTests" )

