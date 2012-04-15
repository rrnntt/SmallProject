#include "QtAPI/FitWidget.h"
#include "ui_FitWidget.h"
#include "QtAPI/TaskManager.h"
#include "QtAPI/SelectFunctionDialog.h"

#include "Numeric/FunctionFactory.h"
#include "Numeric/IFunction.h"
#include "Numeric/CompositeFunction.h"
#include "Numeric/LeastSquares.h"
#include "Numeric/LevenbergMarquardt.h"
#include "Numeric/FunctionDomain1D.h"
#include "Numeric/FunctionValues.h"

#include "DataObjects/TableWorkspace.h"
#include "DataObjects/NumericColumn.h"

#include "API/WorkspaceFactory.h"

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
  connect(m_form->cbWorkspace,SIGNAL(currentIndexChanged()),this,SLOT(fillColumns()));
  connect(m_form->btnFit,SIGNAL(clicked()),this,SLOT(fit()));

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
  fillColumns();
}

void FitWidget::fillColumns()
{
  if (m_form->cbWorkspace->count() == 0)
  {
    return;
  }
  try
  {
    auto ws = API::WorkspaceManager::instance().retrieve(m_form->cbWorkspace->currentText().toStdString());
    auto tws = boost::dynamic_pointer_cast<DataObjects::TableWorkspace>(ws);
    if (!tws) return;
    QStringList colNames;
    std::string xColumn;
    std::string yColumn;
    std::string errColumn;
    // find all numeric columns
    for(size_t i = 0;i<tws->columnCount(); ++i)
    {
      auto col = tws->getColumn(i);
      auto num = col->asNumeric();
      if (num)
      {
        colNames << QString::fromStdString(col->name());
        if (xColumn.empty() && num->getPlotRole() == DataObjects::NumericColumn::X)
        {
          xColumn = col->name();
        }
        if (yColumn.empty() && num->getPlotRole() == DataObjects::NumericColumn::Y)
        {
          yColumn = col->name();
        }
        if (errColumn.empty() && num->getPlotRole() == DataObjects::NumericColumn::yError)
        {
          errColumn = col->name();
        }
      }
    }

    if (xColumn.empty() || yColumn.empty())
    {
      foreach(QString qname,colNames)
      {
        std::string name = qname.toStdString();
        if (xColumn.empty())
        {
          if (name != yColumn)
          {
            xColumn = name;
          }
        }
        if (yColumn.empty())
        {
          if (name != xColumn)
          {
            yColumn = name;
          }
        }
      }
    }
    colNames.prepend("");
    m_form->cbXColumn->insertItems(0,colNames);
    if (!xColumn.empty())
    {
      m_form->cbXColumn->setCurrentIndex(m_form->cbXColumn->findText(QString::fromStdString(xColumn)));
    }
    m_form->cbYColumn->insertItems(0,colNames);
    if (!yColumn.empty())
    {
      m_form->cbYColumn->setCurrentIndex(m_form->cbYColumn->findText(QString::fromStdString(yColumn)));
    }
    m_form->cbErrColumn->insertItems(0,colNames);
    if (!errColumn.empty())
    {
      m_form->cbErrColumn->setCurrentIndex(m_form->cbErrColumn->findText(QString::fromStdString(errColumn)));
    }
  }
  catch(...)
  {
    return;
  }
}

void FitWidget::fit()
{
  std::string wsName = m_form->cbWorkspace->currentText().toStdString();
  if (wsName.empty()) return;
  try
  {
    auto ws = API::WorkspaceManager::instance().retrieve(wsName);
    auto tws = boost::dynamic_pointer_cast<DataObjects::TableWorkspace>(ws);
    if (!tws) throw std::runtime_error("Table workspace not found");
    std::string xColumn = m_form->cbXColumn->currentText().toStdString();
    if (xColumn.empty()) throw std::runtime_error("X column was not set");
    std::string yColumn = m_form->cbYColumn->currentText().toStdString();
    if (yColumn.empty()) throw std::runtime_error("Y column was not set");
    std::string errColumn = m_form->cbErrColumn->currentText().toStdString();
    auto xCol = tws->getColumn(xColumn)->asNumeric();
    if (!xCol) throw std::runtime_error("Column " + xColumn + " must be numeric");
    auto yCol = tws->getColumn(yColumn)->asNumeric();
    if (!yCol) throw std::runtime_error("Column " + yColumn + " must be numeric");
    DataObjects::NumericColumn* errCol = nullptr;
    if (!errColumn.empty())
    {
      errCol = tws->getColumn(errColumn)->asNumeric();
      if (!errCol) throw std::runtime_error("Column " + errColumn + " must be numeric");
    }
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> err;
    xCol->fill(x);
    yCol->fill(y);
    if (errCol)
    {
      errCol->fill(err);
    }
    else
    {
      err.resize(y.size(),1.0);
    }
    Numeric::FunctionDomain1D_sptr domain(new Numeric::FunctionDomain1DVector(x));
    Numeric::FunctionValues_sptr values(new Numeric::FunctionValues(*domain));
    values->setFitData(y);
    values->setFitWeights(err);

    std::string ini = m_form->teFunction->toPlainText().toStdString();
    auto fun = Numeric::FunctionFactory::instance().createFitFunction(ini);

    Numeric::LeastSquares* leastSquares = new Numeric::LeastSquares;
    leastSquares->setFittingFunction(fun,domain,values);
    Numeric::LevenbergMarquardt minimizer;
    minimizer.initialize(Numeric::ICostFunction_sptr(leastSquares));

    minimizer.minimize();
    m_expression.reset(fun->asString(true));
    updateEditor();

    auto parsTable = boost::dynamic_pointer_cast<DataObjects::TableWorkspace>(
      API::Workspace_ptr(API::WorkspaceFactory::instance().create("TableWorkspace"))
      );
    parsTable->addColumn("string","Name");
    parsTable->addColumn("double","Value");
    for(size_t i = 0; i < fun->nParams(); ++i)
    {
      int row = parsTable->appendRow();
      parsTable->getColumn(row)->cell<std::string>(i) = fun->parameterName(i);
      parsTable->getColumn(row)->cell<double>(i) = fun->getParameter(i);
    }
    API::WorkspaceManager::instance().addOrReplace(wsName+"_Parameters",parsTable);
  }
  catch(std::exception& e)
  {
    TaskManager::instance().errorMessage(std::string("Fit failed:\n")+e.what());
  }

}

} // QtAPI
