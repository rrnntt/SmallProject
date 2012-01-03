#include "AlgorithmExplorer.h"
#include "ui_AlgorithmExplorer.h"
#include "AlgorithmDialog.h"

#include "API/AlgorithmFactory.h"

#include <QTreeWidget>

#include <iostream>
#include <stdexcept>

AlgorithmExplorer::AlgorithmExplorer(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::AlgorithmExplorer)
{
  ui->setupUi(this);
  connect(ui->btnRun,SIGNAL(clicked()),this,SLOT(run()));

  ui->cbALgorithms->setEditable(true);
  ui->treeAlgorithms->setColumnCount(1);
  QList<QTreeWidgetItem *> treeItems;
  std::vector<std::string> propNames = API::AlgorithmFactory::instance().keys();

  QStringList items;
  int row = 0;
  for(auto name = propNames.begin(); name != propNames.end(); ++name,++row)
  {
    QString qname = QString::fromStdString(*name);
    items << qname;
    QTreeWidgetItem* treeItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList(qname));
    treeItems.append(treeItem);
  }
  ui->cbALgorithms->insertItems(0,items);
  ui->cbALgorithms->setCurrentIndex(-1);
  ui->treeAlgorithms->insertTopLevelItems(0,treeItems);
  connect(ui->treeAlgorithms,SIGNAL(itemClicked(QTreeWidgetItem *, int)),
    this,SLOT(treeItemClicked(QTreeWidgetItem *, int)));
  connect(ui->treeAlgorithms,SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),
    this,SLOT(treeItemDoubleClicked(QTreeWidgetItem *, int)));
}

AlgorithmExplorer::~AlgorithmExplorer()
{
    delete ui;
}

void AlgorithmExplorer::run()
{
  std::string algName = ui->cbALgorithms->currentText().toStdString();
  if (algName.empty() || !API::AlgorithmFactory::instance().exists(algName))
  {
    return;
  }
  AlgorithmDialog dlg(this,algName);
  if (dlg.exec() == QDialog::Accepted)
  {
    dlg.getAlgorithm()->execute();
  }
}

void AlgorithmExplorer::treeItemClicked(QTreeWidgetItem *item, int column)
{
  ui->cbALgorithms->setCurrentIndex(ui->cbALgorithms->findText(item->text(0)));
}

void AlgorithmExplorer::treeItemDoubleClicked(QTreeWidgetItem *item, int column)
{
  ui->cbALgorithms->setCurrentIndex(ui->cbALgorithms->findText(item->text(0)));
  if (ui->cbALgorithms->currentIndex() >= 0)
  {
    run();
  }
}
