#include "gtest/gtest.h"
#include "Kernel/GeometryTypes.h"

#include <iostream>
#include <string>

using namespace Kernel;

class A
{
  double m_x,m_y;
public:
  double x() const {return m_x;}
  double y() const {return m_y;}
  void setX(double _x) {m_x = _x;}
  void setY(double _y) {m_y = _y;}
};

class B
{
  std::pair<double,double> m_data;
public:
  double x() const {return m_data.first;}
  double y() const {return m_data.second;}
  void setX(double _x) {m_data.first = _x;}
  void setY(double _y) {m_data.second = _y;}
};

TEST(GeometryTypesTest, Test)
{
  Point2D<A> a(11,12);
  std::cerr << a.x() << ' ' << a.y() << std::endl;

  Point2D<A> a1(a);
  std::cerr << a1.x() << ' ' << a1.y() << std::endl;

  Point2D<A> a2(1,1);
  a2 = a1;
  std::cerr << a2.x() << ' ' << a2.y() << std::endl;

  Point2D<B> b = a;
  std::cerr << b.x() << ' ' << b.y() << std::endl;

  Vector2D<B> v(1,1);
  a += v;
  std::cerr << a.x() << ' ' << a.y() << std::endl;

}

