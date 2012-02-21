#ifndef MODELING_MODELTASK_H
#define MODELING_MODELTASK_H

#include "Modeling/DllExport.h"
#include "QtAPI/Task.h"

namespace Modeling
{

class MODELING_EXPORT ModelTask: public QtAPI::Task
{
  Q_OBJECT
public:
  ModelTask();
  virtual Type type() const{return Background;}
  virtual QMenu* menu(QtAPI::SubWindow* w = nullptr) const;
  virtual int menuOrder() const {return QtAPI::customMenuOder;}
protected slots:
  void testModel();
protected:
  QAction *m_testModel;
};

} // namespace Modeling
#endif // MODELING_MODELTASK_H
