#include "Modeling/ModelTask.h"
#include "Modeling/OSGWidget.h"

#include "QtAPI/TaskFactory.h"
#include "QtAPI/WindowManager.h"

#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QtGui/QFileDialog>

namespace Modeling
{

DECLARE_TASK(ModelTask);

ModelTask::ModelTask()
{
  m_testModel = new QAction("Test model...",this);
  connect(m_testModel,SIGNAL(triggered()),this,SLOT(testModel()));
}

QMenu* ModelTask::menu(QtAPI::SubWindow* w) const
{
  QMenu* menu = new QMenu("Model");
  menu->addAction(m_testModel);
  return menu;
}

void ModelTask::testModel()
{
  QString fileName = QFileDialog::getOpenFileName(nullptr,
     "Open an OSG file", "", "OSG Files (*.osg *.osgt)");
  if ( !fileName.isEmpty() )
  {
    QFileInfo finfo( fileName );
    QDir::setCurrent( finfo.absoluteDir().absolutePath() );
    OSGWidget* viewWidget = new OSGWidget();
    viewWidget->setGeometry( 100, 100, 800, 600 );
    Model* model = new Model;
    model->load( fileName.toStdString() );
    viewWidget->setModel( model );
    QtAPI::WindowManager::instance().newSubWindow(viewWidget);
  }

}

} // namespace Modeling
