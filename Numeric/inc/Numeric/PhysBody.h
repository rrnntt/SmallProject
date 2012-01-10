#ifndef NUMERIC_PHYSBODY_H
#define NUMERIC_PHYSBODY_H

#include "Numeric/DllExport.h"
#include "Numeric/V3D.h"

namespace Numeric
{
/**
* A physical body.
*/
class NUMERIC_EXPORT PhysBody
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

} // namespace Numeric

#endif /*NUMERIC_PHYSBODY_H*/