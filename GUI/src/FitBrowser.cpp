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
  connect(widget(),SIGNAL(unsaved()),this,SLOT(unsaved()));
  connect(widget(),SIGNAL(saved()),this,SLOT(saved()));
}

FitBrowser::~FitBrowser()
{
}

void FitBrowser::saved()
{
  setWindowTitle("Fit browser");
}

void FitBrowser::unsaved()
{
  setWindowTitle("Fit browser *");
}
