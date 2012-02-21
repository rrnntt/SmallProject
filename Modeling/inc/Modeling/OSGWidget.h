#ifndef OSGVIEWWIDGET_H
#define OSGVIEWWIDGET_H

#include "Modeling/DllExport.h"
#include "Modeling/Model.h"

#include <QtCore/QTimer>
#include <QtGui/QApplication>
#include <QtGui/QGridLayout>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/TrackballManipulator>

#include <osgDB/ReadFile>

#include <osgQt/GraphicsWindowQt>

namespace Modeling
{

class MODELING_EXPORT OSGWidget : public QWidget, public osgViewer::Viewer
{
public:
    OSGWidget();

    void setModel(Model* model);
    
protected:
    
    osg::Camera* createCamera( int x, int y, int w, int h, const std::string& name="", bool windowDecoration=false );
    
    virtual void paintEvent( QPaintEvent* event );

    QTimer m_timer;
    osg::ref_ptr<Model> m_model;
};

} // Modeling

#endif /*OSGVIEWWIDGET_H*/
