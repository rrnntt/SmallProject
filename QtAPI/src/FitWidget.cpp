#include "QtAPI/FitWidget.h"
#include "ui_FitWidget.h"
#include "QtAPI/TaskManager.h"

#include "Numeric/FunctionFactory.h"
#include "Numeric/IFunction.h"

#include <QTextEdit>
#include <QMenu>
#include <QAction>
#include <QMouseEvent>
#include <QKeyEvent>

namespace QtAPI
{

/**
 * Constructor.
 */
FitWidget::FitWidget(QWidget* parent):
QWidget(parent),
m_form(new Ui::FitWidget),
m_expression("CompositeFunction()")
{
  m_form->setupUi(this);
  m_form->teFunction->setText(QString::fromStdString(m_expression));
  m_form->teFunction->installEventFilter(this);
  //m_form->teFunction->setReadOnly(true);

  connect(this,SIGNAL(needUpdateExpression()),this,SLOT(updateExpression()),Qt::QueuedConnection);
}

/**
 * Destructor.
 */
FitWidget::~FitWidget()
{
  delete m_form;
}

/**
 * Catch events comming to the text widget.
 */
bool FitWidget::eventFilter(QObject *obj, QEvent *ev)
{
  QTextEdit *te = dynamic_cast<QTextEdit*>(obj);
  if (!te)return QWidget::eventFilter(obj,ev);
  //std::cerr << "event " << ev->type() << std::endl;
  if (ev->type() == QEvent::MouseButtonPress)
  {// mouse event in the function editor
    QMouseEvent *mev = static_cast<QMouseEvent*>(ev);
    if (mev->button() == Qt::RightButton)
    {// right button - context menu
      QMenu *context = new QMenu;

      QAction *action = new QAction("Add function",this);
      connect(action,SIGNAL(triggered()),this,SLOT(addFunction()));
      context->addAction(action);

      context->exec(QCursor::pos());
      ev->accept();
      return true;
    }
  }
  else if (ev->type() == QEvent::ShortcutOverride)
  {
    QKeyEvent *kev = static_cast<QKeyEvent*>(ev);
    if (kev->key() == Qt::Key_Return && kev->modifiers() & Qt::ControlModifier)
    {
      emit needUpdateExpression();
    }
  }
  else if (ev->type() == QEvent::FocusOut)
  {
    std::cerr << "event " << ev->type() << std::endl;
  }
  return QWidget::eventFilter(obj,ev);
}

void FitWidget::updateExpression()
{
  try
  {
    auto fun = Numeric::FunctionFactory::instance().createFitFunction(
      m_form->teFunction->toPlainText().toStdString());
    if (fun)
    {
      m_expression.reset(fun->asString(true));
      m_form->teFunction->setText(QString::fromStdString(m_expression));
    }
    else
    {
      TaskManager::instance().errorMessage("Error in function definition");
    }
  }
  catch(std::exception& e)
  {
    TaskManager::instance().errorMessage(std::string("Error in function:\n")+e.what());
    m_form->teFunction->setText(QString::fromStdString(m_expression));
  }
}

QString FitWidget::getFunction()
{
}

/**
 * Add a top-level function
 */
void FitWidget::addFunction()
{
  //updateExpression();
  std::cerr << "add function" << std::endl; 
  auto fun = Numeric::FunctionFactory::instance().createFitFunction(m_expression);
}

} // QtAPI
