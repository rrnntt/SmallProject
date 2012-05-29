#include "Goblin/EnergyList.h"
#include "API/TableColumn.h"
#include "API/WorkspaceFactory.h"

using namespace API;

namespace Goblin
{

DECLARE_WORKSPACE(EnergyList);

EnergyList::EnergyList()
{
  resetColumns();
}

/// Load from an asci file
void EnergyList::loadAscii(const std::string& fileName)
{
  TableWorkspace::loadAscii(fileName);
  resetColumns();
}

void EnergyList::resetColumns()
{
  auto qColumn = static_cast<TableColumn<VJKG>*>(getOrAddColumn("VJKG","q").get());
  m_q = &qColumn->data();
  auto enColumn = static_cast<TableColumn<double>*>(getOrAddColumn("double","Energy").get());
  m_energy = &enColumn->data();
}

size_t EnergyList::index(const VJKG& qq) const
{
  auto it = std::find(m_q->begin(),m_q->end(),qq);
  if (it == m_q->end()) return size();
  return static_cast<size_t>(it - m_q->begin());
}

} // namespace Goblin
