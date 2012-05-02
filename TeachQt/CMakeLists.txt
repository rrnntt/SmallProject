#
#  TeachQt project
#

set( INC_FILES 
   inc/TeachQt/DllExport.h
   inc/TeachQt/Graph.h
   inc/TeachQt/GraphShape.h
   inc/TeachQt/GeometryTask.h
   )

set( SRC_FILES 
  src/GeometryTask.cpp
  src/Graph.cpp
  src/GraphShape.cpp
  )

set( MOC_HEADERS
   inc/TeachQt/GeometryTask.h
   inc/TeachQt/Graph.h
)

set( UI_FILES
)

qt4_wrap_cpp(MOC_FILES ${MOC_HEADERS})
#qt4_wrap_ui(UI_HEADERS ${UI_FILES})

add_library( TeachQt SHARED ${SRC_FILES} ${INC_FILES} ${MOC_FILES} ${UI_HEADERS} )
include_directories(inc 
../Kernel/inc
../API/inc
../Teach/inc
../QtAPI/inc
../qwt/src
${THIRD_PARTY_INCLUDE_DIR}
)
set_target_properties ( TeachQt PROPERTIES COMPILE_DEFINITIONS IN_TEACHQT )
target_link_libraries(TeachQt
QtAPI Kernel API Teach qwt
)

