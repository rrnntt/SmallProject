#
#  Modeling project
#
#project(Modeling)

set( INC_FILES 
   inc/Modeling/DllExport.h
   inc/Modeling/OSGWidget.h
   inc/Modeling/OSGPhysModel.h
   inc/Modeling/Model.h
   )
set( SRC_FILES 
  src/OSGWidget.cpp
  src/OSGPhysModel.cpp
  src/Model.cpp
  )
  
set( MOC_HEADERS
   inc/Modeling/OSGWidget.h
)

set( UI_FILES
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
#qt4_wrap_ui(UI_HEADERS ${UI_FILES})

add_library( Modeling SHARED ${SRC_FILES} ${INC_FILES} ${MOC_FILES} ${UI_HEADERS} ) 
include_directories(inc ../API/inc ../Numeric/inc ../DataObjects/inc
${THIRD_PARTY_INCLUDE_DIR}
"${QT_QTCORE_INCLUDE_DIR}/.."
${QT_QTCORE_INCLUDE_DIR}
${QT_QTGUI_INCLUDE_DIR}
${QT_QTOPENGL_INCLUDE_DIR}
${OSG_INCUDEDIR}
${OSG_BUILDINCLUDEDIR}
)
set_target_properties ( Modeling PROPERTIES COMPILE_DEFINITIONS IN_MODELING )

target_link_libraries(Modeling
API Numeric DataObjects
)

target_link_libraries(Modeling
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
