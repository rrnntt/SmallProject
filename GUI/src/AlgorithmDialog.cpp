#include "AlgorithmDialog.h"

#include "QtAPI/AlgorithmInputHistory.h"
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
  // create widgets for setting properties
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
        QString value = QtAPI::AlgorithmInputHistory::instance().getPropertyValue(algName, *name);
        if ( !value.isEmpty() )
        {
          int i = chb->findText( value );
          if ( i >= 0 ) chb->setCurrentIndex( i );
        }
        edit = chb;
      }
      else
      {
        QLineEdit* le = new QLineEdit(this);
        QString value = QtAPI::AlgorithmInputHistory::instance().getPropertyValue(algName, *name);
        if ( !value.isEmpty() )
        {
          le->setText( value );
        }
        edit = le;
      }
    }
    else
    {
        QLineEdit* le = new QLineEdit(this);
        QString value = QtAPI::AlgorithmInputHistory::instance().getPropertyValue(algName, *name);
        if ( !value.isEmpty() )
        {
          le->setText( value );
        }
        edit = le;
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
    QString algName = QString::fromStdString( m_algorithm->name() );
    for(auto prop = m_propertyMap.begin(); prop != m_propertyMap.end(); ++prop)
    {
      QWidget* edit = prop.value();
      QString propValue;
      if ( dynamic_cast<QLineEdit*>( edit ) )
      {
        propValue = dynamic_cast<QLineEdit*>( edit )->text();
      }
      else if ( dynamic_cast<QComboBox*>( edit ) )
      {
        propValue = dynamic_cast<QComboBox*>( edit )->currentText();
      }
      m_algorithm->setProperty(prop.key(), propValue.toStdString());
      if ( !propValue.isEmpty() )
      {
        QtAPI::AlgorithmInputHistory::instance().savePropertyValue( algName, QString::fromStdString(prop.key()), propValue );
      }
    }
  }
  catch(std::exception& e)
  {
    QMessageBox::critical((QWidget*)parent(),"Error","Error in setting properties:\n" + QString(e.what()));
  }
}
