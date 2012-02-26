#ifndef TEACH_GEOMETRYTASK_H
#define TEACH_GEOMETRYTASK_H

#include "Teach/DllExport.h"
#include "QtAPI/Task.h"

class QAction;

namespace Teach
{

class Graph;

class TEACH_EXPORT GeometryTask: public QtAPI::Task
{
  Q_OBJECT
public:
  GeometryTask();
  ~GeometryTask(){}
  virtual Type type() const {return Background;}
  virtual QMenu* menu(QtAPI::SubWindow* w = nullptr) const;
  virtual int menuOrder() const {return QtAPI::customMenuOder;}

  Graph* createGraph() const;
public slots:
  void newGraph();
protected:
  mutable QMenu* m_menu;
  QAction *m_newGraph;
};

} // namespace Teach
#endif // TEACH_GEOMETRYTASK_H
