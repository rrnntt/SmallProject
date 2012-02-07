#include "Goblin/LineList.h"
#include "DataObjects/TableColumn.h"
#include "API/WorkspaceFactory.h"

using namespace DataObjects;

namespace Goblin
{

DECLARE_WORKSPACE(LineList);

LineList::LineList()
{
  resetColumns();
}

/// Load from an asci file
void LineList::loadAscii(const std::string& fileName)
{
  TableWorkspace::loadAscii(fileName);
  resetColumns();
}

void LineList::resetColumns()
{
  m_q = nullptr;
  m_q0 = nullptr;
  try
  {
    auto qColumn = static_cast<TableColumn<VJKG>*>(getColumn("q").get());
    if (qColumn)
    {
      m_q = &qColumn->data();
    }
    qColumn = static_cast<TableColumn<VJKG>*>(getColumn("q0").get());
    if (qColumn)
    {
      m_q0 = &qColumn->data();
    }
  }
  catch(...)
  {}
  auto dColumn = static_cast<TableColumn<double>*>(getOrAddColumn("double","Line").get());
  m_line = &dColumn->data();
  dColumn = static_cast<TableColumn<double>*>(getOrAddColumn("double","Height").get());
  m_line = &dColumn->data();
}

void LineList::makeQs()
{
  auto qColumn = static_cast<TableColumn<VJKG>*>(getOrAddColumn("VJKG","q").get());
  m_q = &qColumn->data();
  qColumn = static_cast<TableColumn<VJKG>*>(getOrAddColumn("VJKG","q0").get());
  m_q0 = &qColumn->data();
}

void LineList::assign(size_t i, const VJKG& qup, const VJKG& qlo)
{
  if (!isAssigned())
  {
    makeQs();
  }
  if (i < rowCount())
  {
    (*m_q)[i] = qup;
    (*m_q0)[i] = qlo;
  }
}

} // namespace Goblin
