#
#  Plotter
#

set( INC_FILES 

   inc/AlgorithmExplorer.h
   inc/AlgorithmDialog.h
   #inc/Demo.h
   inc/FitBrowser.h
   inc/MainWindow.h
   inc/ViewTask.h
   inc/WorkspaceExplorer.h
   )
set( SRC_FILES 
  src/AlgorithmExplorer.cpp
  src/AlgorithmDialog.cpp
  #src/Demo.cpp
  src/FitBrowser.cpp
  src/main.cpp
  src/MainWindow.cpp
  src/ViewTask.cpp
  src/WorkspaceExplorer.cpp
  )
  
set( MOC_HEADERS
   inc/AlgorithmExplorer.h
   inc/AlgorithmDialog.h
   inc/FitBrowser.h
   inc/MainWindow.h
   inc/ViewTask.h
   inc/WorkspaceExplorer.h
)

set( UI_FILES
   inc/AlgorithmExplorer.ui
   inc/WorkspaceExplorer.ui
)

qt4_wrap_cpp(MOC_FILES ${MOC_HEADERS})
qt4_wrap_ui(UI_HEADERS ${UI_FILES})

add_executable( Plotter ${SRC_FILES} ${INC_FILES} ${MOC_FILES} ${UI_HEADERS} ) 
include_directories(inc  ../API/inc ../QtAPI/inc ../Numeric/inc ../DataObjects/inc ../qwt/src ../Modeling/inc
"${CMAKE_BINARY_DIR}/GUI"
${THIRD_PARTY_INCLUDE_DIR}
"${QT_QTCORE_INCLUDE_DIR}/.."
${QT_QTCORE_INCLUDE_DIR}
${QT_QTGUI_INCLUDE_DIR}
${QT_QTOPENGL_INCLUDE_DIR}
)

target_link_libraries(Plotter
QtAPI Kernel API Numeric DataObjects qwt Modeling
)

target_link_libraries(Plotter
${QT_QTCORE_LIBRARY}
${QT_QTGUI_LIBRARY}
${QT_QTOPENGL_LIBRARY}
)
