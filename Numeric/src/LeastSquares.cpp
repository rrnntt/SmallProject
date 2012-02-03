#include "Numeric/LeastSquares.h"
#include "Numeric/Jacobian.h"

#include <gsl/gsl_matrix.h>

namespace Numeric
{

class LeastSquaresJacobian: public Jacobian
{
public:
  LeastSquaresJacobian(size_t ny, size_t np)
  {
    m_matrix = gsl_matrix_alloc(ny,np);
  }
  ~LeastSquaresJacobian()
  {
    gsl_matrix_free(m_matrix);
  }
  virtual void set(size_t iY, size_t iP, double value)
  {
    gsl_matrix_set(m_matrix, iY, iP, value);
  }
  virtual double get(size_t iY, size_t iP)
  {
    return gsl_matrix_get(m_matrix, iY, iP);
  }
private:
  gsl_matrix* m_matrix;
};

LeastSquares::LeastSquares()
{
}

void LeastSquares::setDomain(FunctionDomain_ptr domain)
{
  if (!domain->isFitDataSet())
  {
    throw std::runtime_error("Fitting data is not set.");
  }
  m_domain = domain;
}

/// Get i-th parameter
double LeastSquares::getParameter(size_t i)const
{
  return m_function->activeParameter(i);
}

/// Set i-th parameter
void LeastSquares::setParameter(size_t i, const double& value)
{
  m_function->setActiveParameter(i, value);
}

/// Total number of parameters
size_t LeastSquares::nParams()const
{
  return m_function->nActive();
}

/// Evaluate the function
double LeastSquares::eval() const
{
  m_function->function(*m_domain);
  double lsq = 0.0;
  size_t n = m_domain->size();
  for(size_t i = 0; i < n; ++i)
  {
    double calc = m_domain->getCalculated(i);
    double obs = m_domain->getFitData(i);
    double w = m_domain->getFitWeight(i);
    double x = (obs - calc);
    lsq += w * x * x;
  }
  return lsq;
}

/// Evaluate the derivatives
void LeastSquares::deriv(std::vector<double>& der) const
{
  size_t np = m_function->nActive();
  size_t ny = m_domain->size();
  LeastSquaresJacobian jac(ny,np);
  m_function->function(*m_domain);
  m_function->functionDeriv(*m_domain,jac);
  if (der.size() != np)
  {
    der.resize(np);
  }
  for(size_t ip = 0; ip < np; ++ip)
  {
    double d = 0.0;
    for(size_t i = 0; i < ny; ++i)
    {
      double calc = m_domain->getCalculated(i);
      double obs = m_domain->getFitData(i);
      double w = m_domain->getFitWeight(i);
      d += w * (obs - calc) * jac.get(i,ip);
    }
    der[ip] = -2*d;
  }
}

/// Evaluate the function and the derivatives
double LeastSquares::evalAndDeriv(std::vector<double>& der) const
{
  size_t np = m_function->nActive();
  size_t ny = m_domain->size();
  LeastSquaresJacobian jac(ny,np);
  m_function->function(*m_domain);
  m_function->functionDeriv(*m_domain,jac);
  if (der.size() != np)
  {
    der.resize(np);
  }
  double lsq = 0.0;
  for(size_t ip = 0; ip < np; ++ip)
  {
    double d = 0.0;
    for(size_t i = 0; i < ny; ++i)
    {
      double calc = m_domain->getCalculated(i);
      double obs = m_domain->getFitData(i);
      double w = m_domain->getFitWeight(i);
      double x = obs - calc;
      if (ip == 0)
      {
        lsq += w * x * x;
      }
      d += w * x * jac.get(i,ip);
    }
    der[ip] = -2*d;
  }
  return lsq;
}

} // namespace Numeric
