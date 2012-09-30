#include "Numeric/Quadrature.h"
#include "Numeric/GSLMatrix.h"

#include <boost/lexical_cast.hpp>

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
  m_funs.resize( size() );
  m_derivs.resize( size() );
  for(size_t i = 0; i < size(); ++i)
  {
    std::string cName = "f" + boost::lexical_cast<std::string>( i );
    m_funs[i] = &getDoubleData( cName );
    cName[0] = 'd';
    m_derivs[i] = &getDoubleData( cName );
  }
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
  const size_t n = size();
  std::vector<double>& d1 = *m_derivs[i];
  std::vector<double>& d2 = *m_derivs[j];
  double res = 0.0;
  for(size_t k = 0; k < n; ++k)
  {
    res += d1[k] * d2[k];
  }
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
  const size_t n = size();
  std::vector<double>& f1 = *m_funs[i];
  std::vector<double>& f2 = *m_funs[j];
  double res = 0;
  for(size_t k = 0; k < n; ++k)
  {
    res += f1[k] * f2[k] * vpot[k];
  }
  return res;
}

/// Build a hamiltonian matrix
void Quadrature::buildHamiltonian(const double& beta, const std::vector<double>& vpot, GSLMatrix& H) const
{
  const size_t n = size();
  H.resize( n, n );
  for(size_t i =0; i < n; ++i)
  {
    for(size_t j =i; j < n; ++j)
    {
      double tmpK = calcKinet( i, j, beta );
      double tmpP = calcPot( i, j, vpot );
      H.set(i,j, tmpK + tmpP);
      if ( i != j ) H.set(j,i, tmpK + tmpP);
    }
  }
}


} // Numeric
