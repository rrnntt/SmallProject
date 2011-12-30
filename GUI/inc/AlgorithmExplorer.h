#ifndef ALGORITHMEXPLORER_H
#define ALGORITHMEXPLORER_H

#include <QDockWidget>

class QAction;

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
private:
  Ui::AlgorithmExplorer *ui;
  QAction *m_run;
};

#endif // ALGORITHMEXPLORER_H
