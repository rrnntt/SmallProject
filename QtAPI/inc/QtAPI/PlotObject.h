#ifndef QTAPI_PLOTOBJECT_H
#define QTAPI_PLOTOBJECT_H

#include "QtAPI/DllExport.h"
#include "API/Workspace.h"

#include <QRectF>
#include <QPen>

class QPainter;
class QwtScaleMap;

namespace QtAPI
{

/**
 * An object that can be plotted on Plot and be part of a PlotWorkspace
 */
class QTAPI_EXPORT PlotObject
{
public:
  /// Plot object id type, objects are identified by ids rather than pointers. Start with 1, 0 means all objects
  typedef size_t id_t;
  PlotObject();
  virtual ~PlotObject(){}

  /// If an object has a connection to a workspace, it can return a pointer to it.
  /// Useful for interactions with workspaces via Plot and pickers.
  virtual API::Workspace_ptr getWorkspace() const {return API::Workspace_ptr();}
  /*!
  \brief Draw the item

  \param painter Painter
  \param xMap Maps x-values into pixel coordinates.
  \param yMap Maps y-values into pixel coordinates.
  \param canvasRect Contents rect of the canvas in painter coordinates
  */
  virtual void drawObject(QPainter *painter, 
    const QwtScaleMap &xMap, const QwtScaleMap &yMap,
    const QRect &canvasRect) const = 0;
  /// Return the bounding rectangle
  virtual QRectF boundingRect() const = 0;

  void setPen(const QPen& pen) {m_pen = pen;}
  const QPen& pen() const {return m_pen;}

protected:

  QPen m_pen;

};

} // QTAPI

#endif // QTAPI_PLOTOBJECT_H
