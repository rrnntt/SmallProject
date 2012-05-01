#ifndef TEACH_SIMULATEPARTICLE_H
#define TEACH_SIMULATEPARTICLE_H

#include "Teach/DllExport.h"
#include "API/Algorithm.h"

namespace Teach
{

/**
 * Simulates motion of a particle: finds and returns the trajectory equation.
 */
class TEACH_EXPORT SimulateParticle: public API::Algorithm
{
public:
  SimulateParticle();
  virtual std::string name() const {return "SimulateParticle";}
protected:
  virtual void exec();
};

} // TEACH

#endif // TEACH_SIMULATEPARTICLE_H
