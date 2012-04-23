#include "QtAPI/PlotWorkspace.h"

#include "API/WorkspaceFactory.h"

#include <boost/lexical_cast.hpp>

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
 * Add new object to the workspace.
 * @param obj :: Pointer to the new object.
 * @return :: The id assigned to the new object.
 */
PlotObject::id_t PlotWorkspace::addObject(PlotObject* obj)
{
  PlotObject::id_t id = m_idSeed;
  m_objects.insert(id, obj);
  ++m_idSeed;
  return id;
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

/// Return the bounding rectangle
QRectF PlotWorkspace::boundingRect() const
{
  QRectF rect;
  for(auto it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    QRectF r = it.value()->boundingRect();
    rect = rect.united(r);
  }
  return rect;
}

} // QtAPI
