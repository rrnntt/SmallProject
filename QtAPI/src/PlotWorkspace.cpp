#include "QtAPI/PlotWorkspace.h"

#include "API/WorkspaceFactory.h"

namespace QtAPI
{

DECLARE_WORKSPACE(PlotWorkspace);

/**
 * Constructor.
 */
PlotWorkspace::PlotWorkspace()
{
}

/**
 * Destructor.
 */
PlotWorkspace::~PlotWorkspace()
{
}

/**
 * Return a pointer to a plot object with given id
 * @param id :: An object id
 * @return :: A pointer to a plot object or nullptr if it doesn't exist
 */
PlotObject* PlotWorkspace::getPlotObject(PlotWorkspace::ObjectID_t id) const
{
  auto o = m_objects.find(id);
  return o != m_objects.end() ? o.value() : nullptr;
}


} // QtAPI
