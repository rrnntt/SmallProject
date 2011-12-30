#include "AlgorithmExplorer.h"
#include "ui_AlgorithmExplorer.h"
#include "AlgorithmDialog.h"

#include "API/AlgorithmFactory.h"

#include <iostream>
#include <stdexcept>

AlgorithmExplorer::AlgorithmExplorer(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::AlgorithmExplorer)
{
  ui->setupUi(this);
  connect(ui->btnRun,SIGNAL(clicked()),this,SLOT(run()));
  ui->cbALgorithms->setEditable(true);
  std::vector<std::string> propNames = API::AlgorithmFactory::instance().keys();
  QStringList items;
  for(auto name = propNames.begin(); name != propNames.end(); ++name)
  {
    items << QString::fromStdString(*name);
  }
  ui->cbALgorithms->insertItems(0,items);
}

AlgorithmExplorer::~AlgorithmExplorer()
{
    delete ui;
}

void AlgorithmExplorer::run()
{
  std::string algName = ui->cbALgorithms->currentText().toStdString();
  std::cerr << "Execute " << algName << std::endl;
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
