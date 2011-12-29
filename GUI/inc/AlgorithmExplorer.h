#ifndef ALGORITHMEXPLORER_H
#define ALGORITHMEXPLORER_H

#include <QDockWidget>

namespace Ui {
    class AlgorithmExplorer;
}

class AlgorithmExplorer : public QDockWidget
{
    Q_OBJECT
public:
    AlgorithmExplorer(QWidget *parent);
    ~AlgorithmExplorer();
private:
    Ui::AlgorithmExplorer *ui;
};

#endif // ALGORITHMEXPLORER_H
