#ifndef NUMERIC_V3D_H
#define NUMERIC_V3D_H

#include "nu/DllExport.h"

namespace nu
{
  /**
   * A vector in 3D space
   */
  class NUMERIC_DLL V3D
  {
  public:
    V3D():m_x(),m_y(),m_z(){}
    V3D(const double& x,const double& y,const double& z):m_x(x),m_y(y),m_z(z){}
    V3D(const V3D& v):m_x(v.m_x),m_y(v.m_y),m_z(v.m_z){}
    V3D& operator=(const V3D& v){m_x=v.m_x;m_y=v.m_y;m_z=v.m_z;return *this;}
    const double& x() const {return m_x;}
    const double& y() const {return m_y;}
    const double& z() const {return m_z;}
    void setX(const double& x) {m_x = x;}
    void setY(const double& y) {m_y = y;}
    void setZ(const double& z) {m_z = z;}
  private:
    double m_x,m_y,m_z;
  };

} // namespace nu

#endif /*NUMERIC_V3D_H*/
