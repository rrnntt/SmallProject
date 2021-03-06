#include "QtAPI/TableDialog.h"
#include "QtAPI/AddTableColumnDialog.h"
//#include "QtAPI/ColumnBox.h"
#include "ui_TableDialog.h"

#include "API/TableWorkspace.h"
#include "API/NumericColumn.h"
#include "API/AlgorithmFactory.h"
//#include "Kernel/WorkspaceProperty.h"

// qwt includes
#include "qwt_scale_div.h"
#include "qwt_text.h"
#include "qwt_legend.h"

#include <QtGui/QItemSelectionModel>
#include <QtGui/QMessageBox>
#include <QtCore/QStringList>

#include <iostream>
#include <stdexcept>

namespace QtAPI
{

const int veryBigRowCount = 100000;

TableDialog::TableDialog(QWidget *parent,boost::shared_ptr<API::TableWorkspace> ws) :
    QDialog(parent),
    ui(new Ui::TableDialog),
    m_workspace(ws),
    m_formulaChangesSaved(false)
{
    ui->setupUi(this);

    init();
}

TableDialog::~TableDialog()
{
    delete ui;
}

void TableDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/**
 * Initialize the dialog with info form the table workspace.
 */
void TableDialog::init()
{
  initColumns();
  selectedColumn();
  initFormulaPage();
  initDistributionPage();
  initFunctionPage();
  connect(ui->cbDistribution,SIGNAL(currentIndexChanged(int)),this,SLOT(updateDistributionPage(int)));
  connect(ui->buttonBox,SIGNAL(clicked(QAbstractButton*)),this,SLOT(applyButton(QAbstractButton*)));
  connect(ui->btnAddColumn,SIGNAL(clicked()),this,SLOT(addColumn()));
  connect(ui->btnRemoveColumn,SIGNAL(clicked()),this,SLOT(removeColumn()));
}

void TableDialog::initColumns()
{
  ui->sbRowCount->setValue(m_workspace->rowCount());
  ui->sbColumnCount->setValue(m_workspace->columnCount());
  ui->sbColumnCount->setEnabled(false);

  // fill in the table of column info
  ui->tableColumns->setRowCount(0);
  std::vector<std::string> colNames = m_workspace->getColumnNames();
  for(auto name = colNames.begin(); name != colNames.end(); ++name)
  {
    auto column = m_workspace->getColumn(*name);
    int row = ui->tableColumns->rowCount();
    ui->tableColumns->insertRow(row);
    ui->tableColumns->model()->setData(ui->tableColumns->model()->index(row,0),QString::fromStdString(*name));
    ui->tableColumns->model()->setData(ui->tableColumns->model()->index(row,1),QString::fromStdString(column->type()));
  }
}

#define __add_to_tableVariables(type,name) \
  ui->tableVariables->insertRow(row); \
  ui->tableVariables->model()->setData(ui->tableVariables->model()->index(row,0),type); \
  ui->tableVariables->model()->setData(ui->tableVariables->model()->index(row,1),name); \
  ++row;


/**
 * Initial setup of the formula page.
 */
void TableDialog::initFormulaPage()
{
  int row = 0;
  //std::vector<std::string> colNames = m_workspace->getColumnNames();
  //for(auto name = colNames.begin(); name != colNames.end(); ++name)
  //{
  //  auto column = m_workspace->getColumn(*name);
  //  if (column->type() == "double")
  //  {
  //    ui->tableVariables->insertRow(n);
  //  }
  //}
  __add_to_tableVariables("Scalar","row");
  __add_to_tableVariables("Scalar","i");
  __add_to_tableVariables("Scalar","pi");
  __add_to_tableVariables("Scalar","e");
  connect(ui->textEdit,SIGNAL(textChanged()),this,SLOT(formulaChanged()));
}

#undef __add_to_tableVariables

/**
 * Initial setup of the distribution page.
 */
void TableDialog::initDistributionPage()
{
  QStringList dist;
  dist << "" << "Serial numbers" << "Uniform" << "Poisson" << "Normal";
  ui->cbDistribution->addItems(dist);
  updateDistributionPage(0);
}

/**
 * Initial setup of the Function page.
 */
void TableDialog::initFunctionPage()
{
    ui->columnBox->setTableWorkspace(m_workspace,false,API::NumericColumn::X);
}

/**
 * Udate the distribution page when different distribution selected.
 * Parameter - unused curent index of cbDistribution
 */
void TableDialog::updateDistributionPage(int)
{
  QString distrName = ui->cbDistribution->currentText();

  if (distrName == "Serial numbers")
  {
    ui->leDistrParam1->setEnabled(true);
    ui->leDistrParam2->setEnabled(false);
    ui->leDistrParam1->setText("1");
    ui->lblDistrParam1->setText("Start");
    ui->lblDistrParam1->show(); ui->leDistrParam1->show();
    ui->lblDistrParam2->hide(); ui->leDistrParam2->hide();
  }
  else if (distrName == "Uniform")
  {
    ui->leDistrParam1->setEnabled(true);
    ui->leDistrParam2->setEnabled(true);
    ui->leDistrParam1->setText("0");
    ui->leDistrParam2->setText("1");
    ui->lblDistrParam1->setText("Minimum");
    ui->lblDistrParam2->setText("Maximum");
    ui->lblDistrParam1->show(); ui->leDistrParam1->show();
    ui->lblDistrParam2->show(); ui->leDistrParam2->show();
  }
  else if (distrName == "Poisson")
  {
    ui->leDistrParam1->setEnabled(true);
    ui->leDistrParam2->setEnabled(false);
    ui->leDistrParam1->setText("1");
    ui->lblDistrParam1->setText("Mean");
    ui->lblDistrParam1->show(); ui->leDistrParam1->show();
    ui->lblDistrParam2->hide(); ui->leDistrParam2->hide();
  }
  else if (distrName == "Normal")
  {
    ui->leDistrParam1->setEnabled(true);
    ui->leDistrParam2->setEnabled(true);
    ui->leDistrParam1->setText("0");
    ui->leDistrParam2->setText("1");
    ui->lblDistrParam1->setText("Mean");
    ui->lblDistrParam2->setText("Variance");
    ui->lblDistrParam1->show(); ui->leDistrParam1->show();
    ui->lblDistrParam2->show(); ui->leDistrParam2->show();
  }
  else
  {
    ui->lblDistrParam1->hide(); ui->leDistrParam1->hide();
    ui->lblDistrParam2->hide(); ui->leDistrParam2->hide();
  }
}

/**
 * Returns index of a column in the workspace selected for modification.
 */
int TableDialog::selectedColumn()
{
  QItemSelectionModel* selection = ui->tableColumns->selectionModel();
  if (!selection->hasSelection())
  {
    ui->tableColumns->selectRow(0);
    return 0;
  }
  QList<QTableWidgetItem *> items = ui->tableColumns->selectedItems();
  if (items.isEmpty())
  {
    ui->tableColumns->selectRow(0);
    return 0;
  }
  return items[0]->row();
}

/**
 * Apply a button from the button box.
 */
void TableDialog::applyButton(QAbstractButton* button)
{
  if (ui->buttonBox->standardButton(button) == QDialogButtonBox::Apply)
  {
    apply();
    m_workspace->modified();
  }
}

/**
 * Set the number of rows in the table to the number in the sbRowColumn 
 */
void TableDialog::applyRowCount()
{
  int nRows = ui->sbRowCount->value();
  if (nRows == m_workspace->rowCount())
  {
    return;
  }
  if (nRows <= 0)
  {
    QMessageBox::critical(this,"Error","Number of rows must be positive.");
    return;
  }
  if (nRows > veryBigRowCount)
  {
    if (QMessageBox::question(this,"Confirmation","Number of rows is very large, are you sure it is correct?","Yes","No")
      != QMessageBox::Accepted)
    {
      return;
    }
  }
  m_workspace->setRowCount(nRows);
  m_workspace->modified();
}

void TableDialog::accept()
{
  apply();
  close();
}

void TableDialog::apply()
{
  applyRowCount();
  int tabIndex = ui->tabWidget->currentIndex();
  QString tabName = ui->tabWidget->tabText(tabIndex);
  if (tabName == "Distribution")
  {
    applyDistribution();
  }
  else if (tabName == "Formula")
  {
    applyFormula();
  }
  else if (tabName == "Function")
  {
    applyFunction();
  }
}

void TableDialog::applyDistribution()
{
  auto column = m_workspace->getColumn(selectedColumn());
  if (!column->isNumeric())
  {
    QMessageBox::critical(parentWidget(),"Error","Column " + QString::fromStdString(column->name()) + " is not numeric");
    return;
  }
  auto numeric = boost::dynamic_pointer_cast<API::NumericColumn>(column);
  size_t n = column->size();
  QString distrName = ui->cbDistribution->currentText();
  if (distrName == "Serial numbers")
  {
    int start = ui->leDistrParam1->text().toInt();
    for(size_t i = 0; i < n; ++i)
    {
      numeric->setDouble(i,static_cast<double>(start + i));
    }
    m_workspace->modified();
  }
  try
  {
    API::Algorithm_ptr alg;
    if (distrName == "Uniform")
    {
      alg = API::AlgorithmFactory::instance().createAlgorithm("CreateUniformRandomData");
      alg->setProperty("Workspace", m_workspace);
      alg->setProperty("MinValue", ui->leDistrParam1->text().toStdString());
      alg->setProperty("MaxValue", ui->leDistrParam2->text().toStdString());
      alg->setProperty("Column", column->name());
    }
    else if (distrName == "Poisson")
    {
      alg = API::AlgorithmFactory::instance().createAlgorithm("CreateDistribution");
      alg->setProperty("Workspace", m_workspace);
      alg->setProperty("Column", column->name());
      alg->setProperty("Distribution", "poisson");
      alg->setProperty("Mean", ui->leDistrParam1->text().toStdString());
      alg->setProperty("Sigma", ui->leDistrParam2->text().toStdString());
    }
    else if (distrName == "Normal")
    {
      alg = API::AlgorithmFactory::instance().createAlgorithm("CreateDistribution");
      alg->setProperty("Workspace", m_workspace);
      alg->setProperty("Column", column->name());
      alg->setProperty("Distribution", "normal");
      alg->setProperty("Mean", ui->leDistrParam1->text().toStdString());
      alg->setProperty("Sigma", ui->leDistrParam2->text().toStdString());
    }

    if (!alg) return;
    alg->execute();
  }
  catch(std::exception& e)
  {
    QMessageBox::critical(parentWidget(),"Error",QString("Algorithm failed \n")+e.what());
    return;
  }
}

/**
 * Apply the formula entered in the text area of the Formula page to calculate values in a column.
 */
void TableDialog::applyFormula()
{
    if ( m_formulaChangesSaved ) return;
  auto column = m_workspace->getColumn(selectedColumn());
  if (!column->isNumeric())
  {
    QMessageBox::critical(parentWidget(),"Error","Column " + QString::fromStdString(column->name()) + " is not numeric");
    return;
  }

  std::string expStr = ui->textEdit->toPlainText().toStdString();
  if (expStr.empty()) return;
  try
  {
    //m_workspace->fillColumn(column->name(),expStr);
    auto alg = API::AlgorithmFactory::instance().createAlgorithm("CalculateColumnValues");
    alg->setProperty("Workspace", m_workspace);
    alg->setProperty("Column", column->name());
    alg->setProperty("Formula", expStr);
    alg->execute();
  }
  catch(std::exception& e)
  {
    QMessageBox::critical(parentWidget(),"Error",QString("Formula failed \n")+e.what());
  }
  m_formulaChangesSaved = true;
}

/**
  Apply the function set in the Function page.
  */
void TableDialog::applyFunction()
{
    auto column = m_workspace->getColumn(selectedColumn());
    if ( !column ) return;
    try
    {
        auto fun = ui->functionBrowser->getFunction();
        API::Algorithm_ptr alg = API::AlgorithmFactory::instance().createAlgorithm("AddFunctionValuesToTable");
        alg->setProperty("Workspace", m_workspace);
        alg->setProperty("Function", fun);
        alg->setProperty("XColumn",ui->columnBox->currentText().toStdString());
        alg->setProperty("YColumn",column->name());
        //alg->setProperty();
        alg->execute();
    }
    catch(std::exception& e)
    {
        QMessageBox::critical(parentWidget(),"Error",QString("Function failed \n")+e.what());
    }
}

void TableDialog::addColumn()
{
  AddTableColumnDialog dlg(this);
  if (dlg.exec() == QDialog::Accepted)
  {
    m_workspace->addColumn(dlg.getType(),dlg.getName());
    m_workspace->modified();
    initColumns();
  }
}

void TableDialog::removeColumn()
{
    int col = selectedColumn();
    std::vector<std::string> names = m_workspace->getColumnNames();
    if (col >= static_cast<int>(names.size())) return;
    std::string columnName = names[col];
    int ans =  QMessageBox::question(this,"Confirmation","Do you really want to remove column "+QString::fromStdString(columnName)+"?","Yes","No");
    if (ans != 0)
    {
      std::cerr << "Answer " << ans <<std::endl;
      return;
    }
    m_workspace->removeColumn(columnName);
    m_workspace->modified();
    initColumns();
}

/**
  Respond on changes in the formula text area. Marks the formula as unsaved.
  */
void TableDialog::formulaChanged()
{
    m_formulaChangesSaved = false;
}

} // QtAPI
