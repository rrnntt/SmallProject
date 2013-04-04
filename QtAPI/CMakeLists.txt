#
#  QtAPI project
#

set( INC_FILES 
   inc/QtAPI/DllExport.h
   inc/QtAPI/AlgorithmInputHistory.h
   inc/QtAPI/ColorBox.h
   inc/QtAPI/FunctionCurve.h
   inc/QtAPI/Plot.h
   inc/QtAPI/PlotCurve.h
   inc/QtAPI/PlotDefaults.h
   inc/QtAPI/PlotDialog.h
   inc/QtAPI/PlotItem.h
   inc/QtAPI/PlotObject.h
   inc/QtAPI/PlotPicker.h
   inc/QtAPI/PlotRescaler.h
   inc/QtAPI/PlotTask.h
   inc/QtAPI/PlotWorkspace.h
   inc/QtAPI/PatternBox.h
   inc/QtAPI/PenStyleBox.h
   inc/QtAPI/SubWindow.h
   inc/QtAPI/AddTableColumnDialog.h
   inc/QtAPI/FileTask.h
   inc/QtAPI/FitWidget.h
   inc/QtAPI/FunctionBrowser.h
   inc/QtAPI/SelectFunctionDialog.h
   inc/QtAPI/SymbolBox.h
   inc/QtAPI/Task.h
   inc/QtAPI/Table.h
   inc/QtAPI/TableDialog.h
   inc/QtAPI/TableTask.h
   inc/QtAPI/TaskFactory.h
   inc/QtAPI/TaskManager.h
   inc/QtAPI/CurveManager.h
   inc/QtAPI/WindowManager.h
   )
set( SRC_FILES 
  src/AlgorithmInputHistory.cpp
  src/ColorBox.cpp
  src/FunctionCurve.cpp
  src/Plot.cpp
  src/PlotCurve.cpp
  src/PlotDefaults.cpp
  src/PlotDialog.cpp
  src/PlotItem.cpp
  src/PlotObject.cpp
  src/PlotPicker.cpp
  src/PlotRescaler.cpp
  src/PlotTask.cpp
  src/PlotWorkspace.cpp
  src/PatternBox.cpp
  src/PenStyleBox.cpp
  src/SelectFunctionDialog.cpp
  src/SubWindow.cpp
  src/AddTableColumnDialog.cpp
  src/SymbolBox.cpp
  src/Task.cpp
  src/FileTask.cpp
  src/FitWidget.cpp
  src/FunctionBrowser.cpp
  src/Table.cpp
  src/TableDialog.cpp
  src/TableTask.cpp
  src/TaskFactory.cpp
  src/TaskManager.cpp
  src/CurveManager.cpp
  src/WindowManager.cpp
  )
  
set( MOC_HEADERS
   inc/QtAPI/ColorBox.h
   inc/QtAPI/PlotCurve.h
   inc/QtAPI/PlotDialog.h
   inc/QtAPI/PatternBox.h
   inc/QtAPI/PenStyleBox.h
   inc/QtAPI/SubWindow.h
   inc/QtAPI/AddTableColumnDialog.h
   inc/QtAPI/CurveManager.h
   inc/QtAPI/FitWidget.h
   inc/QtAPI/FunctionBrowser.h
   inc/QtAPI/Plot.h
   inc/QtAPI/PlotPicker.h
   inc/QtAPI/PlotTask.h
   inc/QtAPI/SelectFunctionDialog.h
   inc/QtAPI/SymbolBox.h
   inc/QtAPI/Table.h
   inc/QtAPI/TableDialog.h
   inc/QtAPI/TableTask.h
   inc/QtAPI/FileTask.h
   inc/QtAPI/Task.h
   inc/QtAPI/WindowManager.h
)

set( UI_FILES
   inc/QtAPI/FitWidget.ui
   inc/QtAPI/PlotDialog.ui
   inc/QtAPI/AddTableColumnDialog.ui
   inc/QtAPI/SelectFunctionDialog.ui
   inc/QtAPI/TableDialog.ui
)

qt4_wrap_cpp(MOC_FILES ${MOC_HEADERS})
qt4_wrap_ui(UI_HEADERS ${UI_FILES})

add_library( QtAPI SHARED ${SRC_FILES} ${INC_FILES} ${MOC_FILES} ${UI_HEADERS} ) 
include_directories(inc ../API/inc ../Kernel/inc ../qwt/src ../DataObjects/inc ../Formula/inc ../Numeric/inc
"${QT_PROPERTY_BROWSER_INCLUDE}"
"${CMAKE_BINARY_DIR}/QtAPI"
${THIRD_PARTY_INCLUDE_DIR}
"${QT_QTCORE_INCLUDE_DIR}/.."
${QT_QTCORE_INCLUDE_DIR}
${QT_QTGUI_INCLUDE_DIR}
${QT_QTOPENGL_INCLUDE_DIR}
)
set_target_properties ( QtAPI PROPERTIES COMPILE_DEFINITIONS IN_QTAPI )

target_link_libraries(QtAPI
Kernel API DataObjects Formula Numeric qwt
)

target_link_libraries(QtAPI
QtPropertyBrowser
${QT_QTCORE_LIBRARY}
${QT_QTGUI_LIBRARY}
${QT_QTOPENGL_LIBRARY}
)
