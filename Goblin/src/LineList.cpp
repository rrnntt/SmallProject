#include "Goblin/LineList.h"
#include "API/TableColumn.h"
#include "API/WorkspaceFactory.h"

#include <sstream>

using namespace API;

namespace Goblin
{

DECLARE_WORKSPACE(LineList);

LineList::LineList()
{
  resetColumns();
  setCurveStyle(Sticks);
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
  auto dColumn = static_cast<NumericTableColumn<double>*>(getOrAddColumn("double","Line").get());
  m_line = &dColumn->data();
  dColumn->setPlotRole(NumericColumn::X);
  if (this->hasColumn("Height"))
  {
    dColumn = static_cast<NumericTableColumn<double>*>(getOrAddColumn("double","Height").get());
    m_height = &dColumn->data();
  }
  else if (this->hasColumn("Intensity"))
  {
    dColumn = static_cast<NumericTableColumn<double>*>(getOrAddColumn("double","Intensity").get());
    m_height = &dColumn->data();
  }
  else
  {
    dColumn = static_cast<NumericTableColumn<double>*>(getOrAddColumn("double","Height").get());
    m_height = &dColumn->data();
  }
  dColumn->setPlotRole(NumericColumn::Y);
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

/**
 * Find index of a line nearest to a frequency x.
 * @param x :: Frequency to look around.
 * @return :: index of found line. A valid index guaranteed (if the linelist isn't empty)
 */
size_t LineList::findNearest(const double& x) const
{
  if (!m_line || m_line->empty()) return 0; // <----------------------! or throw?
  // assume m_line is sorted
  auto it = std::lower_bound(m_line->begin(),m_line->end(),x);
  if (it == m_line->begin()) return 0;
  if (it == m_line->end()) return m_line->size() - 1;
  if (fabs(x - *(it-1)) < fabs(x - *it)) it--;
  return static_cast<size_t>(it - m_line->begin());
}

/**
 */
std::string LineList::lineString(size_t i) const
{
  std::ostringstream ostr;
  ostr << line(i) << ' ' << height(i) << ' ' ;
  if (this->isAssigned())
  {
    ostr << q(i) << ' ' << q0(i);
  }
  return ostr.str();
}

} // namespace Goblin
