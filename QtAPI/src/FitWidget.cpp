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

#include "API/TableWorkspace.h"
#include "API/NumericColumn.h"
#include "API/StringColumn.h"

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
m_form(new Ui::FitWidget)
{
  m_form->setupUi(this);

  connect(this,SIGNAL(needUpdateExpression()),this,SLOT(updateExpression()),Qt::QueuedConnection);
  connect(this,SIGNAL(needUpdateWorkspaces()),this,SLOT(fillWorkspaces()),Qt::QueuedConnection);
  connect(m_form->cbWorkspace,SIGNAL(currentIndexChanged(int)),this,SLOT(fillColumns(int)));
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


QString FitWidget::getFunction()
{
  auto dlg = new SelectFunctionDialog(this);
  if (dlg->exec() == QDialog::Accepted)
  {

  }
  return "";
}


void FitWidget::fillWorkspaces()
{
  m_form->cbWorkspace->blockSignals(true);
  if (m_form->cbWorkspace->count() > 0)
    m_form->cbWorkspace->clear();
  auto wss = API::WorkspaceManager::instance().getAllOf<API::TableWorkspace>();
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

void FitWidget::fillColumns(int)
{
  if (m_form->cbWorkspace->count() == 0)
  {
    return;
  }
  try
  {
    auto ws = API::WorkspaceManager::instance().retrieve(m_form->cbWorkspace->currentText().toStdString());
    auto tws = boost::dynamic_pointer_cast<API::TableWorkspace>(ws);
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
        if (xColumn.empty() && num->getPlotRole() == API::NumericColumn::X)
        {
          xColumn = col->name();
        }
        if (yColumn.empty() && num->getPlotRole() == API::NumericColumn::Y)
        {
          yColumn = col->name();
        }
        if (errColumn.empty() && num->getPlotRole() == API::NumericColumn::yError)
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
    m_form->cbXColumn->clear();
    m_form->cbYColumn->clear();
    m_form->cbErrColumn->clear();
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
    auto tws = boost::dynamic_pointer_cast<API::TableWorkspace>(ws);
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
    API::NumericColumn* errCol = nullptr;
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

    auto fun = m_form->funBrowser->getFunction();

    Numeric::LeastSquares* leastSquares = new Numeric::LeastSquares;
    leastSquares->setFittingFunction(fun,domain,values);
    Numeric::LevenbergMarquardt minimizer;
    minimizer.initialize(Numeric::ICostFunction_sptr(leastSquares));

    minimizer.minimize();

    // create a table with parameters
    auto parsTable = boost::dynamic_pointer_cast<API::TableWorkspace>(
      API::Workspace_ptr(API::WorkspaceFactory::instance().create("TableWorkspace"))
      );
    parsTable->addColumn("string","Name");
    parsTable->addColumn("double","Value");
    parsTable->setRowCount(fun->nParams());
    auto nameColumn = static_cast<API::StringColumn*>(parsTable->getColumn(0).get());
    auto& names = nameColumn->data();
    auto valueColumn = static_cast<API::TableColumn<double>*>(parsTable->getColumn(1).get());
    auto& vals = valueColumn->data();
    for(size_t i = 0; i < fun->nParams(); ++i)
    {
      names[i] = fun->parameterName(i);
      vals[i] = fun->getParameter(i);
    }
    API::WorkspaceManager::instance().addOrReplace(wsName+"_Parameters",parsTable);

    // create a table with comparison data
    auto diffTable = boost::dynamic_pointer_cast<API::TableWorkspace>(
      API::Workspace_ptr(API::WorkspaceFactory::instance().create("TableWorkspace"))
      );
    diffTable->addColumn("double",xColumn);
    diffTable->addColumn("double",yColumn);
    diffTable->addColumn("double","Calc");
    diffTable->addColumn("double","Diff");
    diffTable->setRowCount(x.size());
    auto diffXColumn = static_cast<API::TableColumn<double>*>(diffTable->getColumn(xColumn).get());
    auto& xs = diffXColumn->data();
    auto diffYColumn = static_cast<API::TableColumn<double>*>(diffTable->getColumn(yColumn).get());
    auto& ys = diffYColumn->data();
    auto diffCalcColumn = static_cast<API::TableColumn<double>*>(diffTable->getColumn("Calc").get());
    auto& cals = diffCalcColumn->data();
    auto diffDiffColumn = static_cast<API::TableColumn<double>*>(diffTable->getColumn("Diff").get());
    auto& diffs = diffDiffColumn->data();
    assert(x.size() == xs.size());
    
    for(size_t i = 0; i < x.size(); ++i)
    {
      xs[i] = x[i];
      ys[i] = y[i];
      cals[i] = values->getCalculated(i);
      diffs[i] = ys[i] - cals[i];
    }
    diffXColumn->asNumeric()->setPlotRole(API::NumericColumn::X);
    diffYColumn->asNumeric()->setPlotRole(API::NumericColumn::Y);
    diffCalcColumn->asNumeric()->setPlotRole(API::NumericColumn::Y);
    diffDiffColumn->asNumeric()->setPlotRole(API::NumericColumn::Y);
    API::WorkspaceManager::instance().addOrReplace(wsName+"_Calc",diffTable);
  }
  catch(std::exception& e)
  {
    TaskManager::instance().errorMessage(std::string("Fit failed:\n")+e.what());
  }

}

} // QtAPI
