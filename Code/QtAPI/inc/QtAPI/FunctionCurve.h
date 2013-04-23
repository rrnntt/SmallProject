#ifndef QTAPI_FUNCTIONCURVE_H
#define QTAPI_FUNCTIONCURVE_H

#include "QtAPI/DllExport.h"
#include "QtAPI/PlotObject.h"

#include "Numeric/FunctionDomain1D.h"
#include "Numeric/FunctionValues.h"
#include "Numeric/IFunction.h"

#include "qwt_symbol.h"

#include <QVector>

namespace QtAPI
{

/**
 * Implements a curve defined by a function (not necessarily Formula::Expression) meaning that 
 * for each x there is exactly one y value.
 */
class QTAPI_EXPORT FunctionCurve: public PlotObject
{
public:
  enum CurveStyle
  {
    NoCurve,

    Lines,
    Sticks,
    Steps,
    Dots,

    UserCurve = 100
  };

  FunctionCurve();
  FunctionCurve(const FunctionCurve& right);
  ~FunctionCurve();

  /// "Virtual copy constructor"
  virtual PlotObject* clone() const;

  virtual API::Workspace_ptr getWorkspace() const {return m_workspace;}
  void setWorkspace(API::Workspace_ptr workspace) {m_workspace = workspace;}

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
  /// Set the curve style
  void setCurveStyle(CurveStyle style) {m_curveStyle = style;}
  /// Get the curve style
  CurveStyle getCurveStyle() const { return m_curveStyle;}
  /// Set new symbol type and appearance (size and colour)
  void setSymbol(const QwtSymbol &s);
  /// Get current symbol
  const QwtSymbol& symbol() const;

protected:

  /// calculate and cache the current bounfing rect
  void calcBoundingRect();
  /// Draw the symbols
  void drawSymbols(QPainter *painter, size_t istart, size_t iend) const;
  /// Draw curve in Lines style
  void drawLines(QPainter *painter, 
    const QwtScaleMap &xMap, const QwtScaleMap &yMap,
    const QRect &canvasRect) const;
  /// Draw curve in Sticks style
  void drawSticks(QPainter *painter, 
    const QwtScaleMap &xMap, const QwtScaleMap &yMap,
    const QRect &canvasRect) const;
  /// Draw curve in Steps style
  void drawSteps(QPainter *painter, 
    const QwtScaleMap &xMap, const QwtScaleMap &yMap,
    const QRect &canvasRect) const;
  /// Draw curve in Dots style
  void drawDots(QPainter *painter, 
    const QwtScaleMap &xMap, const QwtScaleMap &yMap,
    const QRect &canvasRect) const;
  /// Draw curve in NoCurve style
  void drawNoCurve(QPainter *painter, 
    const QwtScaleMap &xMap, const QwtScaleMap &yMap,
    const QRect &canvasRect) const;

  /// Store the x values
  Numeric::FunctionDomain1D_sptr m_x;
  /// Store the y values
  Numeric::FunctionValues m_y;
  /// cache the bounding rect
  mutable QRectF m_boundingRect;
  /// Pointer to a workspace the curve can be assiciated with
  API::Workspace_ptr m_workspace;
  /// Curve style
  CurveStyle m_curveStyle;
  /// Data point symbol
  QwtSymbol* m_symbol;
  /// Temp cache for data point co-ordinates (used for drawing the symbols)
  mutable QVector<QPoint> m_pointCoords;
};

} // QTAPI

#endif // QTAPI_FUNCTIONCURVE_H
