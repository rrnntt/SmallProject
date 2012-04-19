#ifndef QTAPI_PLOTWORKSPACE_H
#define QTAPI_PLOTWORKSPACE_H

#include "QtAPI/DllExport.h"
#include "QtAPI/PlotObject.h"

#include "API/Workspace.h"

#include <boost/weak_ptr.hpp>

#include <QHash>

namespace QtAPI
{

/**
 * A collection of PlotObject-s
 */
class QTAPI_EXPORT PlotWorkspace: public API::Workspace
{
public:
  PlotWorkspace();
  ~PlotWorkspace();
  /// Return workspace identifier
  virtual std::string id()const {return "PlotWorkspace";}
  /// Return a pointer to a plot object with given id
  PlotObject* getPlotObject(PlotObject::id_t id) const;
  virtual void draw(QPainter *painter, 
    const QwtScaleMap &xMap, const QwtScaleMap &yMap,
    const QRect &canvasRect) const;
protected:
  /// Store the plot objects in a map
  QHash<PlotObject::id_t, PlotObject*> m_objects;
  PlotObject::id_t m_idSeed;
};

/// Define a shared pointer to the workspace
typedef boost::shared_ptr<PlotWorkspace> PlotWorkspace_sptr;
typedef boost::weak_ptr<PlotWorkspace> PlotWorkspace_wptr;

} // QTAPI

#endif // QTAPI_PLOTWORKSPACE_H
