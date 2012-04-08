#ifndef FITBROWSER_H
#define FITBROWSER_H

#include <QDockWidget>

class QAction;
class QTreeWidgetItem;

class FitBrowser: public QDockWidget
{
  Q_OBJECT
public:
  FitBrowser(QWidget *parent);
  ~FitBrowser();
};

#endif // FITBROWSER_H
