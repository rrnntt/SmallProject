#include "Numeric/Quadrature.h"

namespace Numeric
{

/**
 * Constructor.
 */
Quadrature::Quadrature()
{
}

/**
 * Destructor.
 */
Quadrature::~Quadrature()
{
}

/**--------------------------------------------------------------------------------
 * Initialize the internals.
 */
void Quadrature::init()
{
  m_r = &getDoubleData("r");
  m_w = &getDoubleData("w");
}

/**--------------------------------------------------------------------------------
  * Calculate a matrix element of the kinetic energy.
  * @param i :: The row index
  * @param j :: The column index
  * @param beta :: The coefficient at the kinetic energy operator in the Schrodinger equation.
  * @return The value of the matrix element
  */
double Quadrature::calcKinet(size_t i, size_t j, const double& beta) const
{
  //const size_t n = w.size();
  //std::vector<double>& d1 = *d1f[i];
  //std::vector<double>& d2 = *d1f[j];
  double res = 0.0;
  //for(size_t k = 0; k < n; ++k)
  //{
  //  res += d1[k] * d2[k];
  //}
  res *= beta/2;
  return res;
}

/**----------------------------------------------------------------------------
  * Calculate a matrix element of the potential energy.
  * @param i :: The row index
  * @param j :: The column index
  * @param vpot :: A vector of the values of the potential energy
  * @return The value of the matrix element
  */
double Quadrature::calcPot(size_t i, size_t j, const std::vector<double>& vpot) const
{
  //const size_t n = w.size();
  //std::vector<double>& f1 = *ff[i];
  //std::vector<double>& f2 = *ff[j];
  double res = 0;
  //for(size_t k = 0; k < n; ++k)
  //{
  //  res += f1[k] * f2[k] * vpot[k];
  //}
  return res;
}


} // Numeric
