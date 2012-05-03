#
#  WebGUI
#

set( INC_FILES 

   inc/MainWindow.h
   inc/MyApplication.h
   )
set( SRC_FILES 
  src/main.cpp
  src/MainWindow.cpp
  src/MyApplication.cpp
  )
  
set( MOC_HEADERS
   inc/MainWindow.h
)

set( UI_FILES
)

qt4_wrap_cpp(MOC_FILES ${MOC_HEADERS})
qt4_wrap_ui(UI_HEADERS ${UI_FILES})

add_executable( WebGUI ${SRC_FILES} ${INC_FILES} ${MOC_FILES} ${UI_HEADERS} ) 
include_directories(inc  ../API/inc ../QtAPI/inc ../Numeric/inc ../DataObjects/inc ../qwt/src ../Modeling/inc
"${CMAKE_BINARY_DIR}/GUI"
${THIRD_PARTY_INCLUDE_DIR}
"${QT_QTCORE_INCLUDE_DIR}/.."
${QT_QTCORE_INCLUDE_DIR}
${QT_QTGUI_INCLUDE_DIR}
${QT_QTOPENGL_INCLUDE_DIR}
${QT_QTWEBKIT_INCLUDE_DIR}
)

target_link_libraries(WebGUI
QtAPI Kernel API Numeric DataObjects qwt Modeling
)

target_link_libraries(WebGUI
${QT_QTCORE_LIBRARY}
${QT_QTGUI_LIBRARY}
${QT_QTOPENGL_LIBRARY}
${QT_QTWEBKIT_LIBRARY}
)
