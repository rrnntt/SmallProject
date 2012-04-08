#include "FitBrowser.h"
#include "QtAPI/FitWidget.h"

#include <QTreeWidget>
#include <QMenu>
#include <QAction>

#include <iostream>
#include <stdexcept>

FitBrowser::FitBrowser(QWidget *parent) :
    QDockWidget(parent)
{
  setWidget(new QtAPI::FitWidget(this));
  setWindowTitle("Fit browser");
}

FitBrowser::~FitBrowser()
{
}

