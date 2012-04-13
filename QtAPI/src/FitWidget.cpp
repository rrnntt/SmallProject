#include "QtAPI/FitWidget.h"
#include "ui_FitWidget.h"
#include "QtAPI/TaskManager.h"
#include "QtAPI/SelectFunctionDialog.h"

#include "Numeric/FunctionFactory.h"
#include "Numeric/IFunction.h"
#include "Numeric/CompositeFunction.h"

#include "DataObjects/TableWorkspace.h"

#include <QTextEdit>
#include <QMenu>
#include <QAction>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QStringList>

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
  connect(m_form->teFunction,SIGNAL(textChanged()),this,SIGNAL(unsaved()));
  connect(this,SIGNAL(needUpdateWorkspaces()),this,SLOT(fillWorkspaces()),Qt::QueuedConnection);

  m_form->sbMaxIterations->setValue(500);
  fillWorkspaces();

  setHandler(this,&FitWidget::handleAdd);
  setHandler(this,&FitWidget::handleDelete);
  API::WorkspaceManager::instance().notificationCenter.addObserver(this);
}

/**
 * Destructor.
 */
FitWidget::~FitWidget()
{
  delete m_form;
  API::WorkspaceManager::instance().notificationCenter.removeObserver(this);
}

void FitWidget::handleAdd(const API::WorkspaceManager::AddNotification& nt)
{
  needUpdateWorkspaces();
}

void FitWidget::handleDelete(const API::WorkspaceManager::DeleteNotification& nt)
{
  needUpdateWorkspaces();
}

/**
 * Checks if a function name is registered.
 * @param fName :: Function name to test.
 */
bool FitWidget::isFunction(const std::string& fName) const
{
  return Numeric::FunctionFactory::instance().exists(fName);
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
      auto cursor = m_form->teFunction->cursorForPosition(mev->pos());
      m_form->teFunction->setTextCursor(cursor);
      int textPos = cursor.position();
      if (textPos >= m_expression.size())
      {
        textPos = 0;
      }
      auto& pparser = m_expression.parser(textPos);

      if ( isFunction(pparser.name()) )
      {
        auto cf = boost::dynamic_pointer_cast<Numeric::CompositeFunction>(Numeric::FunctionFactory::instance().createFitFunction(pparser.name()));
        if (cf)
        {
          QMenu *context = new QMenu;

          QAction *action = new QAction("Add function",this);
          connect(action,SIGNAL(triggered()),this,SLOT(addFunction()));
          context->addAction(action);

          context->exec(QCursor::pos());
        }
      }

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
    //std::cerr << "event " << ev->type() << std::endl;
  }
  return QWidget::eventFilter(obj,ev);
}

/**
 * Update m_expression to match the formula editor contents
 */
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
  emit saved();
}

/**
 * Update the formula editor contents to match m_expression
 */
void FitWidget::updateEditor()
{
  m_form->teFunction->setText(QString::fromStdString(m_expression));
  emit saved();
}

QString FitWidget::getFunction()
{
  auto dlg = new SelectFunctionDialog(this);
  if (dlg->exec() == QDialog::Accepted)
  {

  }
  return "";
}

/**
 * Add a top-level function
 */
void FitWidget::addFunction()
{
  // text cursor position in the formula editor
  int textPos = m_form->teFunction->textCursor().position();
  if (textPos >= m_expression.size())
  {
    textPos = 0;
  }
  auto& pparser = m_expression.parser(textPos);

  auto dlg = new SelectFunctionDialog(this);
  if (dlg->exec() == QDialog::Accepted)
  {
    std::string fnName = dlg->getSelection().function;
    auto f = Numeric::FunctionFactory::instance().createFitFunction(fnName);
    if (f->name() == "UserFunction1D")
    {
      std::string formula = dlg->getSelection().formula;
      if ( formula.empty() )
      {
        formula = "a + b * x";
      }
      f->setAttributeValue("Formula",formula);
    }
    //updateExpression();
    auto fun = Numeric::FunctionFactory::instance().createFitFunction(pparser.str());
    auto cf = boost::dynamic_pointer_cast<Numeric::CompositeFunction>(fun);
    if (!cf)
    {
      cf = boost::dynamic_pointer_cast<Numeric::CompositeFunction>(
        Numeric::FunctionFactory::instance().createFitFunction("CompositeFunction"));
      cf->addFunction(fun);
    }
    cf->addFunction(f);

    std::string expr = m_expression;
    expr.replace(pparser.getStartPos(),pparser.getStringSize(),cf->asString());
    fun = Numeric::FunctionFactory::instance().createFitFunction(expr);
    m_expression.reset(fun->asString(true));
    updateEditor();
  }
}

void FitWidget::fillWorkspaces()
{
  m_form->cbWorkspace->blockSignals(true);
  if (m_form->cbWorkspace->count() > 0)
    m_form->cbWorkspace->clear();
  auto wss = API::WorkspaceManager::instance().getAllOf<DataObjects::TableWorkspace>();
  QStringList qNames;
  std::cerr << "Names:\n";
  for(auto ws = wss.begin(); ws != wss.end(); ++ws)
  {
    QString name = QString::fromStdString((**ws).name());
    qNames << name;
    std::cerr << (**ws).name() << std::endl;
  }
  m_form->cbWorkspace->insertItems(0,qNames);
  m_form->cbWorkspace->blockSignals(false);
}

} // QtAPI
