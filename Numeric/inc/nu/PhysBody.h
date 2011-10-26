#ifndef NUMERIC_PHYSBODY_H
#define NUMERIC_PHYSBODY_H

#include "nu/DllExport.h"
#include "nu/V3D.h"

namespace nu
{
/**
* A physical body.
*/
class NUMERIC_DLL PhysBody
{
public:
  PhysBody(double mass):m_mass(mass),m_pos(),m_velocity(),m_acceleration(){}
  const double& getMass() const {return m_mass;}
protected:
  double m_mass;
  V3D m_pos;
  V3D m_velocity;
  V3D m_acceleration;
};

} // namespace nu

#endif /*NUMERIC_PHYSBODY_H*/