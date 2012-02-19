#ifndef QTAPI_PLOTCURVE_H
#define QTAPI_PLOTCURVE_H

#include "DllExport.h"
#include "API/Workspace.h"
#include "qwt_plot_curve.h"

#include <QPointer>

namespace QtAPI
{

class QTAPI_EXPORT PlotCurve: public QObject,  public QwtPlotCurve
{
  Q_OBJECT
public:
  ~PlotCurve();
  static QPointer<PlotCurve> create();
  static QPointer<PlotCurve> create(const QString &title);
  void setWorkspace(API::Workspace_ptr workspace){m_workspace = workspace;}
  API::Workspace_ptr getWorkspace() const {return m_workspace;}
protected:
  PlotCurve();
  PlotCurve(const QString &title);
  API::Workspace_ptr m_workspace;
};

} // QtAPI

#endif // QTAPI_PLOTCURVE_H
