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
  m_form->pteLog->setReadOnly(true);
  connect(m_form->btnLoadLineList,SIGNAL(clicked()),this,SLOT(loadLineList()));
  connect(m_form->btnLoadLowerEnergies,SIGNAL(clicked()),this,SLOT(loadEnergyList()));
  connect(m_form->btnCreateUpperEnergies,SIGNAL(clicked()),this,SLOT(createEnergyList()));

  connect(m_form->btnSet,SIGNAL(clicked()),this,SLOT(setLists()));
  connect(m_form->btnFind,SIGNAL(clicked()),this,SLOT(find()));
  updateLineListCB();
  updateEnergyListCBs();
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

void ComDiffDialog::setLists()
{
  std::string llName = m_form->cbLineList->currentText().toStdString();
  LineList_ptr ll = boost::dynamic_pointer_cast<LineList>(API::WorkspaceManager::instance().retrieve(llName));

  std::string leName = m_form->cbLowerEnergy->currentText().toStdString();
  EnergyList_ptr le = boost::dynamic_pointer_cast<EnergyList>(API::WorkspaceManager::instance().retrieve(leName));

  std::string ueName = m_form->cbUpperEnergy->currentText().toStdString();
  EnergyList_ptr ue = boost::dynamic_pointer_cast<EnergyList>(API::WorkspaceManager::instance().retrieve(ueName));

  m_cd.sp = ll;
  m_cd.low_ener = le;
  m_cd.up_ener = ue;
}

void ComDiffDialog::find()
{
  std::string upqstr = m_form->leUpperLevel->text().toStdString();
  VJKG upq;
  upq.assign(upqstr);
  double upperEnergy = m_form->leUpperEnergy->text().toDouble();
  std::cerr << upq << ' ' << upperEnergy << std::endl;
  //m_cd.set(
}
} // namespace Goblin
