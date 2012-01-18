#
#  GUI project
#
#project(Plotter)

set( INC_FILES 
   inc/AddTableColumnDialog.h
   inc/AlgorithmExplorer.h
   inc/AlgorithmDialog.h
   inc/Demo.h
   inc/MainWindow.h
   inc/FileTask.h
   inc/ViewTask.h
   inc/Task.h
   inc/Table.h
   inc/TableDialog.h
   inc/TaskFactory.h
   inc/TaskManager.h
   inc/CurveManager.h
   inc/WorkspaceExplorer.h
   )
set( SRC_FILES 
  src/AddTableColumnDialog.cpp
  src/AlgorithmExplorer.cpp
  src/AlgorithmDialog.cpp
  src/Demo.cpp
  src/main.cpp
  src/MainWindow.cpp
  src/Task.cpp
  src/FileTask.cpp
  src/ViewTask.cpp
  src/Table.cpp
  src/TableDialog.cpp
  src/TaskFactory.cpp
  src/TaskManager.cpp
  src/CurveManager.cpp
  src/WorkspaceExplorer.cpp
  )
  
set( MOC_HEADERS
   inc/AddTableColumnDialog.h
   inc/AlgorithmExplorer.h
   inc/AlgorithmDialog.h
   inc/CurveManager.h
   inc/MainWindow.h
   inc/Table.h
   inc/TableDialog.h
   inc/FileTask.h
   inc/ViewTask.h
   inc/Task.h
   inc/WorkspaceExplorer.h
)

set( UI_FILES
   inc/AddTableColumnDialog.ui
   inc/AlgorithmExplorer.ui
   inc/TableDialog.ui
   inc/WorkspaceExplorer.ui
)

qt4_wrap_cpp(MOC_FILES ${MOC_HEADERS})
qt4_wrap_ui(UI_HEADERS ${UI_FILES})

add_executable( Plotter ${SRC_FILES} ${INC_FILES} ${MOC_FILES} ${UI_HEADERS} ) 
include_directories(inc ../QtAPI/inc ../Numeric/inc ../API/inc ../DataObjects/inc ../qwt/src ../Modeling/inc
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
