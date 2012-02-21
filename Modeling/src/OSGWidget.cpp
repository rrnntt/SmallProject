#include "Modeling/OSGWIdget.h"

#include <QtGui/QPainter>

namespace Modeling
{

/**
 * Constructor.
 */
OSGWidget::OSGWidget() : QWidget()
{
  //osg::Node* scene = osgDB::readNodeFile("cow.osg");
  //setSceneData( scene );

  osg::Camera* camera = createCamera(0,0,100,100);
  setCamera( camera );

  addEventHandler( new osgViewer::StatsHandler );
  setCameraManipulator( new osgGA::TrackballManipulator );

  osgQt::GraphicsWindowQt* gw = dynamic_cast<osgQt::GraphicsWindowQt*>( camera->getGraphicsContext() );
  QWidget* widget1 = gw ? gw->getGLWidget() : NULL;

  QGridLayout* grid = new QGridLayout;
  grid->addWidget( widget1, 0, 0 );
  setLayout( grid );

  connect( &m_timer, SIGNAL(timeout()), this, SLOT(update()) );
  m_timer.start( 10 );
}

/**
 * Create a camera.
 */
osg::Camera* OSGWidget::createCamera( int x, int y, int w, int h, const std::string& name, bool windowDecoration )
{
  osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
  osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
  traits->windowName = name;
  traits->windowDecoration = windowDecoration;
  traits->x = x;
  traits->y = y;
  traits->width = w;
  traits->height = h;
  traits->doubleBuffer = true;
  traits->alpha = ds->getMinimumNumAlphaBits();
  traits->stencil = ds->getMinimumNumStencilBits();
  traits->sampleBuffers = ds->getMultiSamples();
  traits->samples = ds->getNumMultiSamples();

  osg::ref_ptr<osg::Camera> camera = new osg::Camera;
  camera->setGraphicsContext( new osgQt::GraphicsWindowQt(traits.get()) );

  camera->setClearColor( osg::Vec4(0.2, 0.2, 0.6, 1.0) );
  camera->setViewport( new osg::Viewport(0, 0, traits->width, traits->height) );
  camera->setProjectionMatrixAsPerspective(
    30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0f, 10000.0f );
  return camera.release();
}

/**
 * Paint event handler.
 */
void OSGWidget::paintEvent( QPaintEvent* event )
{ 
  frame(); 
}

void OSGWidget::setModel(Model* model)
{
  m_model = model;
  setSceneData( m_model->getNode() );
}

} // Modeling
