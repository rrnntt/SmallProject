#include "MyApplication.h"
#include "QtAPI/TaskManager.h"

/**
 * Constructor.
 */
MyApplication::MyApplication(int argc, char** argv):
QApplication(argc,argv)
{
}

/**
 * Destructor.
 */
MyApplication::~MyApplication()
{
}

bool MyApplication::notify( QObject *receiver, QEvent *ev )
{
  try
  {
    return QApplication::notify(receiver,ev);
  }
  catch(std::exception& e)
  {
    QtAPI::TaskManager::instance().errorMessage(std::string("Unexpected exception:\n") + e.what());
  }
  return true;
}
