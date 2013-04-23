#ifndef GOBLIN_DIATOM_H
#define GOBLIN_DIATOM_H

#include "Goblin/DllExport.h"
#include "API/Algorithm.h"

namespace Goblin
{

/**
  \class Diatom.
  \brief Calculates ro-vibraional states of a diatomic molecule.

  Find the solutions of the Schrodinger equation

  \left(-\frac{\hbar^2}{2\mu}\frac{d^2}{dr^2}+V(r)+\frac{\hbar^2 J(J+1)}{2\mu r^2}\right)\psi_{nJ}(r)=E_{nJ}\psi_{nJ}(r)

  Input parameters:
  1. mu - the redused mass of the diatom: \mu=\frac{m_1m_2}{m_1+m_2}
  2. Vpot - the radial potential energy function as a Function1D init string
  3. nmax - the maximum vibrational quantum number
  4. Jmax - the maximum rotational quantum number

  Output parameters:
  1. Energies - a TableWorkspace with the energies
  2. Functions - a TableWorkspace with the functions

 */
class GOBLIN_EXPORT Diatom: public API::Algorithm
{
public:
  Diatom();
  virtual std::string name() const {return "Diatom";}
protected:
  virtual void exec();
};

} // GOBLIN

#endif // GOBLIN_DIATOM_H
