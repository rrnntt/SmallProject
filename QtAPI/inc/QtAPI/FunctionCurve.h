#ifndef QTAPI_FUNCTIONCURVE_H
#define QTAPI_FUNCTIONCURVE_H

#include "QtAPI/DllExport.h"
#include "QtAPI/PlotObject.h"

#include "Numeric/FunctionDomain1D.h"
#include "Numeric/FunctionValues.h"
#include "Numeric/IFunction.h"

namespace QtAPI
{

/**
 * Implements a curve defined by a function (not necessarily Formula::Expression) meaning that 
 * for each x there is exactly one y value.
 */
class QTAPI_EXPORT FunctionCurve: public PlotObject
{
public:
  FunctionCurve();
  ~FunctionCurve();
  /*!
  \brief Draw the item

  \param painter Painter
  \param xMap Maps x-values into pixel coordinates.
  \param yMap Maps y-values into pixel coordinates.
  \param canvasRect Contents rect of the canvas in painter coordinates
  */
  virtual void drawObject(QPainter *painter, 
    const QwtScaleMap &xMap, const QwtScaleMap &yMap,
    const QRect &canvasRect) const;

  /// Return the bounding rectangle
  virtual QRectF boundingRect() const;

  /// Set the curve values from two std vectors
  void setData(const std::vector<double>& x, const std::vector<double>& y);

  /// Set the curve values from IFunction1D
  void setData(const Numeric::FunctionDomain& domain, const Numeric::IFunction& fun);

protected:

  /// calculate and cache the current bounfing rect
  void calcBoundingRect();

  /// Store the x values
  Numeric::FunctionDomain1D_sptr m_x;
  /// Store the y values
  Numeric::FunctionValues m_y;
  /// cache the bounding rect
  mutable QRectF m_boundingRect;
};

} // QTAPI

#endif // QTAPI_FUNCTIONCURVE_H
