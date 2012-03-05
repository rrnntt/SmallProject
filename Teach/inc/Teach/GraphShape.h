#ifndef TEACH_GRAPHSHAPE_H
#define TEACH_GRAPHSHAPE_H

#include "Teach/DllExport.h"

#include "qwt_plot_item.h"

#include <QtGui/QPen>

namespace Teach
{

class TEACH_EXPORT GraphShape: public QwtPlotItem
{
public:
  GraphShape();
  ~GraphShape(){}
  //virtual void draw(QPainter *painter, 
  //    const QwtScaleMap &xMap, const QwtScaleMap &yMap,
  //    const QRect &canvasRect) const = 0;
  void setPen(const QPen& pen) {m_pen = pen;}
protected:
  void beginPainting(QPainter *painter)const;
  void endPainting(QPainter *painter)const;
  QPen m_pen;
};

class TEACH_EXPORT GraphPoint: public GraphShape
{
public:
  GraphPoint();
  virtual void draw(QPainter *painter, 
      const QwtScaleMap &xMap, const QwtScaleMap &yMap,
      const QRect &canvasRect) const;
  void setPoint(const QPointF& p){m_p = p;}
  void setSize(int r){m_size = r;}
protected:
  QPointF m_p;
  int m_size;
};

class TEACH_EXPORT GraphCircle: public GraphShape
{
public:
  GraphCircle();
  virtual void draw(QPainter *painter, 
      const QwtScaleMap &xMap, const QwtScaleMap &yMap,
      const QRect &canvasRect) const;
  void setCentre(double x, double y){m_centre = QPointF(x,y);}
  void setCentre(const QPointF& c){m_centre = c;}
  QPointF getCentre() const {return m_centre;}
  void setRadius(double r){m_radius = r;}
  double getRadius() const {return m_radius;}
protected:
  double m_radius;
  QPointF m_centre;
};

class TEACH_EXPORT GraphLineSegment: public GraphShape
{
public:
  GraphLineSegment();
  virtual void draw(QPainter *painter, 
      const QwtScaleMap &xMap, const QwtScaleMap &yMap,
      const QRect &canvasRect) const;
  void setLine(const QPointF& p1, const QPointF& p2);
protected:
  QPointF m_p1;
  QPointF m_p2;
};

} // namespace Teach
#endif // TEACH_GRAPHSHAPE_H
