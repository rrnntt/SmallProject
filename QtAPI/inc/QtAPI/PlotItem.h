#ifndef QTAPI_PLOTITEM_H
#define QTAPI_PLOTITEM_H

#include "QtAPI/DllExport.h"
#include "QtAPI/PlotObject.h"

#include <qwt_plot_item.h>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace QtAPI
{

  class PlotWorkspace;

/**
 * An interface class between Plot and PlotObject. Inherits QwtPlotItem and is managed by Plot.
 * Has a weak reference to PlotObject
 */
class QTAPI_EXPORT PlotItem: public QwtPlotItem
{
public:
  PlotItem(boost::shared_ptr<PlotWorkspace> ws, PlotObject::id_t id = 0);
  ~PlotItem();
  /// Return plot object id
  PlotObject::id_t getID() const {return m_id;}
  /*!
  \brief Draw the item

  \param painter Painter
  \param xMap Maps x-values into pixel coordinates.
  \param yMap Maps y-values into pixel coordinates.
  \param canvasRect Contents rect of the canvas in painter coordinates
  */
  virtual void draw(QPainter *painter, 
    const QwtScaleMap &xMap, const QwtScaleMap &yMap,
    const QRect &canvasRect) const ;
  virtual QwtDoubleRect boundingRect() const;

protected:
  /// Weak pointer to the workspace holding the plot object
  boost::weak_ptr<PlotWorkspace> m_workspace;
  /// The id of the plot object
  PlotObject::id_t m_id;
};

} // QTAPI

#endif // QTAPI_PLOTITEM_H
