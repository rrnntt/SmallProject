#include "Numeric/Distribution.h"

#include <gsl/gsl_randist.h>

#include <cmath>
#include <stdexcept>

namespace Numeric
{

Distribution::Distribution()
{
  m_rnd_gen = gsl_rng_alloc (gsl_rng_taus);
}

Distribution::~Distribution()
{
  gsl_rng_free (m_rnd_gen);
}

//  ---- NormalDistribution ------  //

NormalDistribution::NormalDistribution(double m, double s):
Distribution(),
m_mean(m),
m_sigma(s)
{
}

double NormalDistribution::random() const
{
  double rnd = gsl_ran_gaussian(m_rnd_gen,m_sigma);
  return rnd + m_mean;
}

double NormalDistribution::mean() const
{
  return m_mean;
}

double NormalDistribution::sigma() const
{
  return m_sigma;
}

double NormalDistribution::pdf(double x) const
{
  return gsl_ran_gaussian_pdf (x - m_mean, m_sigma);
}

//  ---- PoissonDistribution ------  //

PoissonDistribution::PoissonDistribution(double m):
Distribution()
{
  if (m <= 0.0)
  {
    throw std::invalid_argument("Mean in Poisson distribution must be positive");
  }
  m_mean = m;
  m_sigma = sqrt(m);
}

double PoissonDistribution::random() const
{
  return gsl_ran_poisson(m_rnd_gen,m_mean);
}

double PoissonDistribution::mean() const
{
  return m_mean;
}

double PoissonDistribution::sigma() const
{
  return m_sigma;
}

double PoissonDistribution::pdf(double x) const
{
  if (x <= 0) return 0;
  // a check for huge x's needed !
  return gsl_ran_poisson_pdf (static_cast<unsigned int>(x), m_mean);
}

} // namespace Numeric
