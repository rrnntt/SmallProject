#include "AlgorithmDialog.h"

#include "API/Algorithm.h"
#include "API/AlgorithmFactory.h"
#include "Kernel/CommonProperties.h"

#include <QGridLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLineEdit>
#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QComboBox>

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
  std::vector<std::string> propertyNames = m_algorithm->getPropertyNames();
  for(auto name = propertyNames.begin(); name != propertyNames.end(); ++name,++row)
  {
    auto& prop = m_algorithm->get(*name);
    QWidget* edit = NULL;
    auto strProp = dynamic_cast<const Kernel::StringProperty*>( &prop );
    if ( strProp )
    {
      auto keys = strProp->getValues();
      if ( !keys.empty() )
      {
        QComboBox* chb = new QComboBox(this);
        QStringList lst;
        for(auto key = keys.begin(); key != keys.end(); ++key)
        {
          lst << QString::fromStdString( *key );
        }
        chb->addItems( lst );
        edit = chb;
      }
      else
      {
        edit = new QLineEdit(this);
      }
    }
    else
    {
      edit = new QLineEdit(this);
    }
    m_propertyMap.insert(*name,edit);
    QLabel* lbl = new QLabel(QString::fromStdString(*name),this);
    propLayout->addWidget(lbl,row,0);
    propLayout->addWidget(edit,row,1);
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
  try
  {
    for(auto prop = m_propertyMap.begin(); prop != m_propertyMap.end(); ++prop)
    {
      QWidget* edit = prop.value();
      if ( dynamic_cast<QLineEdit*>( edit ) )
      {
        m_algorithm->setProperty(prop.key(), dynamic_cast<QLineEdit*>( edit )->text().toStdString());
      }
      else if ( dynamic_cast<QComboBox*>( edit ) )
      {
        m_algorithm->setProperty(prop.key(), dynamic_cast<QComboBox*>( edit )->currentText().toStdString());
      }
    }
  }
  catch(std::exception& e)
  {
    QMessageBox::critical((QWidget*)parent(),"Error","Error in setting properties:\n" + QString(e.what()));
  }
}
