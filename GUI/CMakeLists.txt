#
#  GUI project
#
project(Plotter)

set( INC_FILES 
   inc/AddTableColumnDialog.h
   inc/AlgorithmExplorer.h
   inc/AlgorithmDialog.h
   inc/Demo.h
   inc/MainWindow.h
   inc/OSGWidget.h
   inc/OSGPhysModel.h
   inc/Model.h
   inc/Plot.h
   inc/PlotDialog.h
   inc/PlotDefaults.h
   inc/FileTask.h
   inc/ViewTask.h
   inc/Task.h
   inc/Table.h
   inc/TableDialog.h
   inc/TaskFactory.h
   inc/TaskManager.h
   inc/CurveManager.h
   inc/SubWindow.h
   inc/WorkspaceExplorer.h
   )
set( SRC_FILES 
  src/AddTableColumnDialog.cpp
  src/AlgorithmExplorer.cpp
  src/AlgorithmDialog.cpp
  src/Demo.cpp
  src/main.cpp
  src/MainWindow.cpp
  src/OSGWidget.cpp
  src/OSGPhysModel.cpp
  src/Model.cpp
  src/Plot.cpp
  src/PlotDialog.cpp
  src/PlotDefaults.cpp
  src/Task.cpp
  src/FileTask.cpp
  src/ViewTask.cpp
  src/Table.cpp
  src/TableDialog.cpp
  src/TaskFactory.cpp
  src/TaskManager.cpp
  src/CurveManager.cpp
  src/SubWindow.cpp
  src/WorkspaceExplorer.cpp
  )
  
set( MOC_HEADERS
   inc/AddTableColumnDialog.h
   inc/AlgorithmExplorer.h
   inc/AlgorithmDialog.h
   inc/MainWindow.h
   inc/OSGWidget.h
   inc/PlotDialog.h
   inc/Table.h
   inc/TableDialog.h
   inc/FileTask.h
   inc/ViewTask.h
   inc/Task.h
   inc/SubWindow.h
   inc/WorkspaceExplorer.h
)

set( UI_FILES
   inc/AddTableColumnDialog.ui
   inc/AlgorithmExplorer.ui
   inc/PlotDialog.ui
   inc/TableDialog.ui
   inc/WorkspaceExplorer.ui
)

# OpenSceneGraph source directory
#set( OSG_DIR "C:/Work/OSG/OpenSceneGraph-3.0.0-rc4")
# OSG includes
set( OSG_INCUDEDIR "${OSG_DIR}/include")
set( OSG_BUILDINCLUDEDIR "${OSG_DIR}/../Build/include")
# OSG library directory
set( OSG_LIBDIR "${OSG_DIR}/../Build/lib")
set( OSG_OSG_DEBUG_LIB "${OSG_LIBDIR}/osgd.lib")
set( OSG_OSG_RELEASE_LIB "${OSG_LIBDIR}/osg.lib")
set( OSG_OSG_LIBS "optimized;${OSG_OSG_RELEASE_LIB};debug;${OSG_OSG_DEBUG_LIB};")

set( OSG_GA_DEBUG_LIB "${OSG_LIBDIR}/osgGAd.lib")
set( OSG_GA_RELEASE_LIB "${OSG_LIBDIR}/osgGA.lib")
set( OSG_GA_LIBS "optimized;${OSG_GA_RELEASE_LIB};debug;${OSG_GA_DEBUG_LIB};")

set( OSG_DB_DEBUG_LIB "${OSG_LIBDIR}/osgDBd.lib")
set( OSG_DB_RELEASE_LIB "${OSG_LIBDIR}/osgDB.lib")
set( OSG_DB_LIBS "optimized;${OSG_DB_RELEASE_LIB};debug;${OSG_DB_DEBUG_LIB};")

set( OSG_VIEWER_DEBUG_LIB "${OSG_LIBDIR}/osgViewerd.lib")
set( OSG_VIEWER_RELEASE_LIB "${OSG_LIBDIR}/osgViewer.lib")
set( OSG_VIEWER_LIBS "optimized;${OSG_VIEWER_RELEASE_LIB};debug;${OSG_VIEWER_DEBUG_LIB};")

set( OSG_QT_DEBUG_LIB "${OSG_LIBDIR}/osgQtd.lib")
set( OSG_QT_RELEASE_LIB "${OSG_LIBDIR}/osgQt.lib")
set( OSG_QT_LIBS "optimized;${OSG_QT_RELEASE_LIB};debug;${OSG_QT_DEBUG_LIB};")

set( OSG_OPENTHREADS_DEBUG_LIB "${OSG_LIBDIR}/OpenThreadsd.lib")
set( OSG_OPENTHREADS_RELEASE_LIB "${OSG_LIBDIR}/OpenThreads.lib")
set( OSG_OPENTHREADS_LIBS "optimized;${OSG_OPENTHREADS_RELEASE_LIB};debug;${OSG_OPENTHREADS_DEBUG_LIB};")

set( OSG_ANIMATION_DEBUG_LIB "${OSG_LIBDIR}/osgAnimationd.lib")
set( OSG_ANIMATION_RELEASE_LIB "${OSG_LIBDIR}/osgAnimation.lib")
set( OSG_ANIMATION_LIBS "optimized;${OSG_ANIMATION_RELEASE_LIB};debug;${OSG_ANIMATION_DEBUG_LIB};")

qt4_wrap_cpp(MOC_FILES ${MOC_HEADERS})
qt4_wrap_ui(UI_HEADERS ${UI_FILES})

add_executable( Plotter ${SRC_FILES} ${INC_FILES} ${MOC_FILES} ${UI_HEADERS} ) 
include_directories(inc ../Numeric/inc ../API/inc ../DataObjects/inc ../qwt/src
"${CMAKE_BINARY_DIR}/GUI"
${THIRD_PARTY_INCLUDE_DIR}
"${QT_QTCORE_INCLUDE_DIR}/.."
${QT_QTCORE_INCLUDE_DIR}
${QT_QTGUI_INCLUDE_DIR}
${QT_QTOPENGL_INCLUDE_DIR}
${OSG_INCUDEDIR}
${OSG_BUILDINCLUDEDIR}
)

target_link_libraries(Plotter
Kernel API Numeric DataObjects qwt
)

target_link_libraries(Plotter
${QT_QTCORE_LIBRARY}
${QT_QTGUI_LIBRARY}
${QT_QTOPENGL_LIBRARY}
${OSG_OSG_LIBS}
${OSG_GA_LIBS}
${OSG_DB_LIBS}
${OSG_VIEWER_LIBS}
${OSG_QT_LIBS}
${OSG_OPENTHREADS_LIBS}
${OSG_ANIMATION_LIBS}
)
