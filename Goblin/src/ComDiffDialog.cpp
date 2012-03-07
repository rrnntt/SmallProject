#include "Goblin/ComDiffDialog.h"
#include "ui_ComDiffDialog.h"
#include "Goblin/LineList.h"
#include "Goblin/EnergyList.h"
#include "Goblin/GoblinTask.h"

#include "QtAPI/TaskManager.h"

#include "API/WorkspaceManager.h"

#include <QtGui/QApplication>
#include <QtGui/QMessageBox>

#include <sstream>

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
  setLists();
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
  setLists();
}

void ComDiffDialog::loadLineList()
{
  auto task = QtAPI::TaskManager::instance().getAs<GoblinTask>("GoblinTask");
  if (task)
  {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    task->loadLineList();
    QApplication::restoreOverrideCursor();
  }
  updateLineListCB();
}

void ComDiffDialog::loadEnergyList()
{
  auto task = QtAPI::TaskManager::instance().getAs<GoblinTask>("GoblinTask");
  if (task)
  {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    task->loadEnergyList();
    QApplication::restoreOverrideCursor();
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
  try
  {
    m_cd.sp.reset();
    m_cd.low_ener.reset();
    m_cd.up_ener.reset();

    std::string llName = m_form->cbLineList->currentText().toStdString();
    if (!llName.empty())
    {
      LineList_ptr ll = boost::dynamic_pointer_cast<LineList>(API::WorkspaceManager::instance().retrieve(llName));
      m_cd.sp = ll;
    }

    std::string leName = m_form->cbLowerEnergy->currentText().toStdString();
    if (!leName.empty())
    {
      EnergyList_ptr le = boost::dynamic_pointer_cast<EnergyList>(API::WorkspaceManager::instance().retrieve(leName));
      m_cd.low_ener = le;
    }

    std::string ueName = m_form->cbUpperEnergy->currentText().toStdString();
    if (!ueName.empty())
    {
      EnergyList_ptr ue = boost::dynamic_pointer_cast<EnergyList>(API::WorkspaceManager::instance().retrieve(ueName));
      m_cd.up_ener = ue;
    }

    if (m_cd.sp && m_cd.low_ener && m_cd.up_ener)
    {
      m_form->btnFind->setEnabled(true);
    }
    else
    {
      m_form->btnFind->setEnabled(false);
    }

  }
  catch(std::exception& e)
  {
    QMessageBox::critical(this,"Error","Failed to set the lists");
  }
}

void ComDiffDialog::find()
{
  std::string upqstr = m_form->leUpperLevel->text().toStdString();
  VJKG upq;
  upq.assign(upqstr);
  double upperEnergy = m_form->leUpperEnergy->text().toDouble();
  std::cerr << upq << ' ' << upperEnergy << std::endl;
  m_cd.set(upperEnergy,upq);
  m_cd.find();
  updateForm();
}

void ComDiffDialog::updateForm()
{
  m_form->pteLog->clear();

  std::ostringstream ostr;
  ostr << m_cd;
  m_form->pteLog->insertPlainText(QString::fromStdString(ostr.str()));

  size_t n = m_cd.found.size();
  m_form->cbSelectFound->clear();
  for(size_t i = 0; i < n; ++i)
  {
    m_form->cbSelectFound->insertItem(
  }
}

} // namespace Goblin
