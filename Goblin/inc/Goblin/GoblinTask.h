#ifndef GOBLIN_GOBLINTASK_H
#define GOBLIN_GOBLINTASK_H

#include "Goblin/DllExport.h"
#include "QtAPI/Task.h"

class QAction;

namespace QtAPI
{
  class Table;
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
protected:
  void addTableActions(QMenu* menu, QtAPI::Table* table) const; 

  QAction *m_loadEnergyList;
  QAction *m_loadLineList;
  QAction *m_comdiff;
};

} // namespace Goblin
#endif // GOBLIN_GOBLINTASK_H
