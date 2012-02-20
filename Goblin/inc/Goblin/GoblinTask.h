#ifndef GOBLIN_GOBLINTASK_H
#define GOBLIN_GOBLINTASK_H

#include "Goblin/DllExport.h"
#include "QtAPI/Task.h"

#include <QtCore/QPointer>

class QAction;

namespace QtAPI
{
  class Table;
  class Plot;
}

namespace Goblin
{

class GOBLIN_EXPORT GoblinTask: public QtAPI::Task
{
  Q_OBJECT
public:
  GoblinTask();
  ~GoblinTask(){}
  virtual Type type() const {return Background;}
  virtual QMenu* menu(QtAPI::SubWindow* w = nullptr) const;
  virtual int menuOrder() const {return QtAPI::customMenuOder;}
public slots:
  void createEnergyList();
  void loadEnergyList();
  void loadLineList();
  void comDiff();
protected slots:
  void plotLineList();
  void addLineList();
protected:
  void addTableActions(QMenu* menu, QtAPI::Table* table) const; 
  void addPlotActions(QMenu* menu, QtAPI::Plot* plot) const; 
  mutable QPointer<QtAPI::Table> m_table;
  mutable QPointer<QtAPI::Plot> m_plot;

  QAction *m_loadEnergyList;
  QAction *m_loadLineList;
  QAction *m_comdiff;
  QAction *m_plotLineList;
  QAction *m_addLineList;
};

} // namespace Goblin
#endif // GOBLIN_GOBLINTASK_H
