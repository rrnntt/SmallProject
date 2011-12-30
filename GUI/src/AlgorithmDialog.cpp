#include "AlgorithmDialog.h"

#include "API/Algorithm.h"
#include "API/AlgorithmFactory.h"

#include <QGridLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLineEdit>
#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>

#include <iostream>
#include <stdexcept>

AlgorithmDialog::AlgorithmDialog(QWidget *parent,const std::string& algName) :
    QDialog(parent)
{
  setWindowTitle(windowTitle() + " " + QString::fromStdString(algName));
  QVBoxLayout *mainLayout = new QVBoxLayout(this);

  // setup properties layout
  QGridLayout *propLayout = new QGridLayout();

  m_algorithm = API::AlgorithmFactory::instance().createAlgorithm(algName);
  if (!m_algorithm)
  {
    QMessageBox::critical(parent,"Error","Algorithm " + QString::fromStdString(algName) + " not found.");
    return;
  }
  int row = 0;
  std::set<std::string> propertyNames = m_algorithm->getPropertyNames();
  for(auto name = propertyNames.begin(); name != propertyNames.end(); ++name,++row)
  {
    QLineEdit* le = new QLineEdit(this);
    m_propertyMap.insert(*name,le);
    QLabel* lbl = new QLabel(QString::fromStdString(*name),this);
    propLayout->addWidget(lbl,row,0);
    propLayout->addWidget(le,row,1);
  }

  // setup button layout
  QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                      | QDialogButtonBox::Cancel);

  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  mainLayout->addLayout(propLayout);
  mainLayout->addWidget(buttonBox);
}

void AlgorithmDialog::accept()
{
  QDialog::accept();
  if (!m_algorithm) return;
  for(auto prop = m_propertyMap.begin(); prop != m_propertyMap.end(); ++prop)
  {
    m_algorithm->get(prop.key()) = prop.value()->text().toStdString();
  }
  std::cerr << "Accepted\n";
}
