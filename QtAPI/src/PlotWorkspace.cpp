#include "QtAPI/PlotWorkspace.h"

#include "API/WorkspaceFactory.h"

namespace QtAPI
{

DECLARE_WORKSPACE(PlotWorkspace);

/**
 * Constructor.
 */
PlotWorkspace::PlotWorkspace():
m_idSeed(1)
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
PlotObject* PlotWorkspace::getPlotObject(PlotObject::id_t id) const
{
  auto o = m_objects.find(id);
  return o != m_objects.end() ? o.value() : nullptr;
}

/*!
\brief Draw all objects in the workspace.
\param painter Painter
\param xMap Maps x-values into pixel coordinates.
\param yMap Maps y-values into pixel coordinates.
\param canvasRect Contents rect of the canvas in painter coordinates
*/
void PlotWorkspace::draw(QPainter *painter, const QwtScaleMap &xMap, const QwtScaleMap &yMap,
  const QRect &canvasRect) const 
{
  for(auto it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    it.value()->drawObject(painter,xMap,yMap,canvasRect);
  }
}


} // QtAPI
