#include "AlgorithmDialog.h"

#include "API/Algorithm.h"
#include "API/AlgorithmFactory.h"

#include <QGridLayout>

#include <iostream>
#include <stdexcept>

AlgorithmDialog::AlgorithmDialog(QWidget *parent,const QString& algName) :
    QDialog(parent)
{
  setWindowTitle(windowTitle() + " " + algName);
  QGridLayout *layout = new QGridLayout(this);

  m_algorithm = API::AlgorithmFactory::instance().createAlgorithm(algName.toStdString());
}

