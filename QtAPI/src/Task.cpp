#include "QtAPI/Task.h"

#include <QtGui/QMessageBox>
#include <QtGui/QInputDialog>

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

  void Task::errorMessage(const std::string& mess) const
  {
    QMessageBox::critical(nullptr,"Error",QString::fromStdString(mess));
  }

  std::string Task::getInput(const std::string& mess)const
  {
    bool ok;
    QString text = QInputDialog::getText(NULL, "Input",
      QString::fromStdString(mess), QLineEdit::Normal,"", &ok);
    if (ok && !text.isEmpty())
    {
      return text.toStdString();
    }
    return "";
  }

} // QtAPI
