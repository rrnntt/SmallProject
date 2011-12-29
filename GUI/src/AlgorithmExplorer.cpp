#include "AlgorithmExplorer.h"
#include "ui_AlgorithmExplorer.h"

#include <iostream>
#include <stdexcept>

AlgorithmExplorer::AlgorithmExplorer(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::AlgorithmExplorer)
{

    ui->setupUi(this);
}

AlgorithmExplorer::~AlgorithmExplorer()
{
    delete ui;
}

