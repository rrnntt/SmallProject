#include "QtAPI/Task.h"

#include <iostream>

namespace QtAPI
{

  void Task::addAction(const QString& name, QAction* action)
  {
    if (m_actions.contains(name))
    {
      throw std::runtime_error("Action " + name.toStdString() + " already exists in task");
    }
    m_actions.insert(name,action);
  }

  QAction* Task::getAction(const QString& name) const
  {
    if (m_actions.contains(name))
    {
      return m_actions[name];
    }
    return nullptr;
  }

} // QtAPI
