#ifndef KERNEL_GEOMETRYTYPES_H
#define KERNEL_GEOMETRYTYPES_H

#include "Kernel/DllExport.h"

namespace Kernel
{

/**
 * A vector in a 2D space.
 */
template<class C>
class Vector2D: public C
{
public:
  Vector2D(double _x, double _y):C()
  {
    set(_x,_y);
  }

  template<class S>
  Vector2D(const Vector2D<S>& p):C()
  {
    set(p.x(),p.y());
  }

  template<class S>
  Vector2D& operator=(const Vector2D<S>& p)
  {
    set(p.x(),p.y());
    return *this;
  }

  double x() const {return C::x();}
  double y() const {return C::y();}
  void set(double _x, double _y) {C::setX(_x); C::setY(_y);}
  void setX(double _x) {C::setX(_x);}
  void setY(double _y) {C::setY(_y);}

  Vector2D& operator*=(const double& d)
  {
    setX(x() * d);
    setY(y() * d);
    return *this;
  }

  Vector2D& operator/=(const double& d)
  {
    setX(x() / d);
    setY(y() / d);
    return *this;
  }

  Vector2D& operator+=(const double& d)
  {
    setX(x() + d);
    setY(y() + d);
    return *this;
  }

  Vector2D& operator-=(const double& d)
  {
    setX(x() - d);
    setY(y() - d);
    return *this;
  }

  template<class S>
  Vector2D& operator+=(const Vector2D<S>& p)
  {
    setX(x() + p.x());
    setY(y() + p.y());
    return *this;
  }

  template<class S>
  Vector2D& operator-=(const Vector2D<S>& p)
  {
    setX(x() - p.x());
    setY(y() - p.y());
    return *this;
  }

};

/**
 * A point in a 2D space.
 */
template<class C>
class Point2D: public C
{
public:
  Point2D(double _x, double _y):C()
  {
    set(_x,_y);
  }

  template<class S>
  Point2D(const Point2D<S>& p):C()
  {
    set(p.x(),p.y());
  }

  template<class S>
  Point2D& operator=(const Point2D<S>& p)
  {
    set(p.x(),p.y());
    return *this;
  }

  double x() const {return C::x();}
  double y() const {return C::y();}
  void set(double _x, double _y) {C::setX(_x); C::setY(_y);}
  void setX(double _x) {C::setX(_x);}
  void setY(double _y) {C::setY(_y);}

  Point2D& operator*=(const double& d)
  {
    setX(x() * d);
    setY(y() * d);
    return *this;
  }

  Point2D& operator/=(const double& d)
  {
    setX(x() / d);
    setY(y() / d);
    return *this;
  }

  Point2D& operator+=(const double& d)
  {
    setX(x() + d);
    setY(y() + d);
    return *this;
  }

  Point2D& operator-=(const double& d)
  {
    setX(x() - d);
    setY(y() - d);
    return *this;
  }

  template<class S>
  Point2D& operator+=(const Vector2D<S>& p);

  template<class S>
  Point2D& operator-=(const Vector2D<S>& p);

};

template<class C>
template<class S>
Point2D<C>& Point2D<C>::operator+=(const Vector2D<S>& p)
{
  setX(x() + p.x());
  setY(y() + p.y());
  return *this;
}

template<class C>
template<class S>
Point2D<C>& Point2D<C>::operator-=(const Vector2D<S>& p)
{
  setX(x() - p.x());
  setY(y() - p.y());
  return *this;
}

} // namespace Kernel
#endif // KERNEL_GEOMETRYTYPES_H
