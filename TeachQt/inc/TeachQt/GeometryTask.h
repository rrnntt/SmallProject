#ifndef TEACHQT_GEOMETRYTASK_H
#define TEACHQT_GEOMETRYTASK_H

#include "TeachQt/DllExport.h"
#include "QtAPI/Task.h"

class QAction;

namespace TeachQt
{

class Graph;

class TEACHQT_EXPORT GeometryTask: public QtAPI::Task
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
#endif // TEACHQT_GEOMETRYTASK_H
