#ifndef ALGORITHMEXPLORER_H
#define ALGORITHMEXPLORER_H

#include <QDockWidget>

class QAction;
class QTreeWidgetItem;

namespace Ui {
  class AlgorithmExplorer;
}

class AlgorithmExplorer : public QDockWidget
{
  Q_OBJECT
public:
  AlgorithmExplorer(QWidget *parent);
  ~AlgorithmExplorer();
protected slots:
  void run();
  void treeItemDoubleClicked(QTreeWidgetItem *item, int column);
  void treeItemClicked(QTreeWidgetItem *item, int column);
private:
  Ui::AlgorithmExplorer *ui;
  QAction *m_run;
};

#endif // ALGORITHMEXPLORER_H
