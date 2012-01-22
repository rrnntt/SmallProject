#
#  Goblin
#

set( INC_FILES 
   inc/Goblin/mio.h
   inc/Goblin/cmd.h
   inc/Goblin/object.h
   inc/Goblin/DllExport.h
   inc/Goblin/comdiff.h
   inc/Goblin/dbase.h
   inc/Goblin/d_container.h
   #inc/Goblin/effham.h
   inc/Goblin/enlist.h
   #inc/Goblin/exp_cond.h
   #inc/Goblin/humlicek.h
   #inc/Goblin/ifun.h
   #inc/Goblin/lineparams.h
   #inc/Goblin/matrix.h
   #inc/Goblin/mLDs.h
   #inc/Goblin/ospline.h
   inc/Goblin/spbase.h
   #inc/Goblin/spectrum.h
   inc/Goblin/splist.h
   #inc/Goblin/text.h
   inc/Goblin/utils.h
   inc/Goblin/vjkg.h
   #inc/Goblin/voigt.h
   )
set( SRC_FILES 
  src/mio.cpp
  src/object.cpp
  src/comdiff.cpp
  #src/creating.cpp
  src/dbase.cpp
  #src/effham.cpp
  src/enlist.cpp
  #src/exp_cond.cpp
  #src/humlicek.cpp
  #src/ifun.cpp
  #src/lineparams.cpp
  #src/matrix.cpp
  #src/mLDs.cpp
  #src/ospline.cpp
  src/spbase.cpp
  #src/spectrum.cpp
  src/splist.cpp
  #src/splist_gfit.cpp
  #src/text.cpp
  src/utils.cpp
  src/vjkg.cpp
  #src/voigt.cpp
  )
add_library( Goblin SHARED ${SRC_FILES} ${INC_FILES} )
include_directories(inc ${THIRD_PARTY_INCLUDE_DIR})
set_target_properties ( Goblin PROPERTIES COMPILE_DEFINITIONS IN_GOBLIN )

#set( GSL_DEBUG_LIB "${THIRD_PARTY_LIB_DIR}/gsl_d.lib")
#set( GSL_RELEASE_LIB "${THIRD_PARTY_LIB_DIR}/gsl.lib")
#set( GSL_LIBS "optimized;${GSL_RELEASE_LIB};debug;${GSL_DEBUG_LIB};")

#target_link_libraries(Goblin
#${GSL_LIBS}
#)

#
#  GoblinTest
#

set( INC_FILES )
set( SRC_FILES 
  test/main.cpp
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
