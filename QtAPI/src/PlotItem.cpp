#include "QtAPI/PlotItem.h"

namespace QtAPI
{

/**
 * Constructor.
 * @param ws :: A shared pointer to a plot workspace. A weak poinetr is kept.
 * @param id :: An id of a plot object
 */
PlotItem::PlotItem(PlotWorkspace_sptr ws, PlotWorkspace::ObjectID_t id):
m_workspace(ws),m_id(id)
{
}

/**
 * Destructor.
 */
PlotItem::~PlotItem()
{
}

  /*!
  \brief Draw the item

  \param painter Painter
  \param xMap Maps x-values into pixel coordinates.
  \param yMap Maps y-values into pixel coordinates.
  \param canvasRect Contents rect of the canvas in painter coordinates
  */
  void PlotItem::draw(QPainter *painter, 
    const QwtScaleMap &xMap, const QwtScaleMap &yMap,
    const QRect &canvasRect) const 
  {
  }

} // QtAPI
