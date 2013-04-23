#ifndef NUMERIC_DISTRIBUTION_H
#define NUMERIC_DISTRIBUTION_H

#include "Numeric/DllExport.h"

#include <gsl/gsl_rng.h>

namespace Numeric
{

class NUMERIC_EXPORT Distribution
{
public:
  Distribution();
  virtual ~Distribution();
  virtual double random() const = 0;
  virtual double mean() const = 0;
  virtual double sigma() const = 0;
  virtual double pdf(double x) const = 0;
protected:
  gsl_rng * m_rnd_gen;
};

class NUMERIC_EXPORT NormalDistribution: public Distribution
{
public:
  NormalDistribution(double m, double s);
  virtual double random() const;
  virtual double mean() const;
  virtual double sigma() const;
  virtual double pdf(double x) const;
protected:
  double m_mean;
  double m_sigma;
};

class NUMERIC_EXPORT PoissonDistribution: public Distribution
{
public:
  PoissonDistribution(double m);
  virtual double random() const;
  virtual double mean() const;
  virtual double sigma() const;
  virtual double pdf(double x) const;
protected:
  double m_mean;
  double m_sigma;
};

} // namespace Numeric
#endif // NUMERIC_DISTRIBUTION_H
