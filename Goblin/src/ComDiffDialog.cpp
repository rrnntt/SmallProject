#include "Goblin/ComDiffDialog.h"
#include "ui_ComDiffDialog.h"
#include "Goblin/LineList.h"
#include "Goblin/EnergyList.h"
#include "Goblin/GoblinTask.h"

#include "QtAPI/TaskManager.h"

#include "API/WorkspaceManager.h"

namespace Goblin
{

ComDiffDialog::ComDiffDialog(QWidget *parent):
QDialog(parent),
m_form(new Ui::ComDiffDialog)
{
  m_form->setupUi(this);
  connect(m_form->btnLoadLineList,SIGNAL(clicked()),this,SLOT(loadLineList()));
  connect(m_form->btnLoadLowerEnergies,SIGNAL(clicked()),this,SLOT(loadEnergyList()));
  connect(m_form->btnCreateUpperEnergies,SIGNAL(clicked()),this,SLOT(createEnergyList()));
}

void ComDiffDialog::updateLineListCB()
{
  try
  {
    QString line = m_form->cbLineList->currentText();
    m_form->cbLineList->clear();
    auto lineLists = API::WorkspaceManager::instance().getAllOf<LineList>();
    if (!lineLists.empty())
    {
      for(auto it = lineLists.begin(); it != lineLists.end(); ++it)
      {
        m_form->cbLineList->addItem(QString::fromStdString((**it).name()));
      }
    }
    int i = m_form->cbLineList->findText(line);
    if (i >= 0)
    {
      m_form->cbLineList->setCurrentIndex(i);
    }
  }
  catch(...)
  {}
}

void ComDiffDialog::updateEnergyListCBs()
{
  QString upper = m_form->cbUpperEnergy->currentText();
  QString lower = m_form->cbLowerEnergy->currentText();
  m_form->cbLowerEnergy->clear();
  m_form->cbUpperEnergy->clear();
  auto energies = API::WorkspaceManager::instance().getAllOf<EnergyList>();
  if (!energies.empty())
  {
    for(auto it = energies.begin(); it != energies.end(); ++it)
    {
      m_form->cbLowerEnergy->addItem(QString::fromStdString((**it).name()));
      m_form->cbUpperEnergy->addItem(QString::fromStdString((**it).name()));
    }
  }
  int i = m_form->cbUpperEnergy->findText(upper);
  if (i >= 0)
  {
    m_form->cbUpperEnergy->setCurrentIndex(i);
  }
  i = m_form->cbLowerEnergy->findText(lower);
  if (i >= 0)
  {
    m_form->cbLowerEnergy->setCurrentIndex(i);
  }
}

void ComDiffDialog::loadLineList()
{
  auto task = QtAPI::TaskManager::instance().getAs<GoblinTask>("GoblinTask");
  if (task)
  {
    task->loadLineList();
  }
  updateLineListCB();
}

void ComDiffDialog::loadEnergyList()
{
  auto task = QtAPI::TaskManager::instance().getAs<GoblinTask>("GoblinTask");
  if (task)
  {
    task->loadEnergyList();
  }
  updateEnergyListCBs();
}

void ComDiffDialog::createEnergyList()
{
  auto task = QtAPI::TaskManager::instance().getAs<GoblinTask>("GoblinTask");
  if (task)
  {
    task->createEnergyList();
  }
  updateEnergyListCBs();
}

} // namespace Goblin
