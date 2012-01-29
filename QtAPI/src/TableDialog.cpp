#include "QtAPI/TableDialog.h"
#include "ui_TableDialog.h"

#include "DataObjects/TableWorkspace.h"
#include "DataObjects/NumericColumn.h"
#include "API/AlgorithmFactory.h"

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

TableDialog::TableDialog(QWidget *parent,boost::shared_ptr<DataObjects::TableWorkspace> ws) :
    QDialog(parent),
    ui(new Ui::TableDialog),
    m_workspace(ws)
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
  ui->sbRowCount->setValue(m_workspace->rowCount());
  ui->sbColumnCount->setValue(m_workspace->columnCount());
  //ui->sbRowCount->setEnabled(false);
  ui->sbColumnCount->setEnabled(false);

  // fill in the table of column info
  ui->tableColumns->clear();
  std::vector<std::string> colNames = m_workspace->getColumnNames();
  for(auto name = colNames.begin(); name != colNames.end(); ++name)
  {
    auto column = m_workspace->getColumn(*name);
    int row = ui->tableColumns->rowCount();
    ui->tableColumns->insertRow(row);
    ui->tableColumns->model()->setData(ui->tableColumns->model()->index(row,0),QString::fromStdString(*name));
    ui->tableColumns->model()->setData(ui->tableColumns->model()->index(row,1),QString::fromStdString(column->type()));
  }
  selectedColumn();
  initFormulaPage();
  initDistributionPage();
  connect(ui->cbDistribution,SIGNAL(currentIndexChanged(int)),this,SLOT(updateDistributionPage(int)));
  connect(ui->buttonBox,SIGNAL(clicked(QAbstractButton*)),this,SLOT(applyButton(QAbstractButton*)));
}

/**
 * Initial setup of the formula page.
 */
void TableDialog::initFormulaPage()
{
}

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
    QMessageBox::critical(this,"Error","Number of rows is very large");
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
}

void TableDialog::applyDistribution()
{
  auto column = m_workspace->getColumn(selectedColumn());
  if (!column->isNumeric())
  {
    QMessageBox::critical(parentWidget(),"Error","Column " + QString::fromStdString(column->name()) + " is not numeric");
    return;
  }
  auto numeric = boost::dynamic_pointer_cast<DataObjects::NumericColumn>(column);
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
      alg->get("Workspace") = m_workspace->name();
      alg->get("MinValue") = ui->leDistrParam1->text().toStdString();
      alg->get("MaxValue") = ui->leDistrParam2->text().toStdString();
      alg->get("Column") = column->name();
    }
    else if (distrName == "Poisson")
    {
      alg = API::AlgorithmFactory::instance().createAlgorithm("CreateDistribution");
      alg->get("Workspace") = m_workspace->name();
      alg->get("Column") = column->name();
      alg->get("Distribution") = "poisson";
      alg->get("Mean") = ui->leDistrParam1->text().toStdString();
      alg->get("Sigma") = ui->leDistrParam2->text().toStdString();
    }
    else if (distrName == "Normal")
    {
      alg = API::AlgorithmFactory::instance().createAlgorithm("CreateDistribution");
      alg->get("Workspace") = m_workspace->name();
      alg->get("Column") = column->name();
      alg->get("Distribution") = "normal";
      alg->get("Mean") = ui->leDistrParam1->text().toStdString();
      alg->get("Sigma") = ui->leDistrParam2->text().toStdString();
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

} // QtAPI
