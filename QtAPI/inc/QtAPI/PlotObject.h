#ifndef QTAPI_PLOTOBJECT_H
#define QTAPI_PLOTOBJECT_H

#include "QtAPI/DllExport.h"

class QPainter;
class QwtScaleMap;
class QRect;

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
};

} // QTAPI

#endif // QTAPI_PLOTOBJECT_H
