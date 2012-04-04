#include "Numeric/LeastSquares.h"
#include "Numeric/StdJacobian.h"
#include "Numeric/IConstraint.h"
#include "Numeric/CompositeDomain.h"
#include "Numeric/FunctionValues.h"
#include "Numeric/CostFunctionFactory.h"

namespace
{
  bool debug = false;
}

namespace Numeric
{

DECLARE_COSTFUNCTION(LeastSquares)

/// Calculate value of cost function
/// @return :: The value of the function
double LeastSquares::val() const
{
  if ( !m_dirtyVal ) return m_value;

  checkValidity();

  m_value = 0.0;

  auto compDomain = boost::dynamic_pointer_cast<CompositeDomain>(m_domain);

  auto simpleValues = boost::dynamic_pointer_cast<FunctionValues>(m_values);
  if (!simpleValues)
  {
    throw std::runtime_error("LeastSquares: unsupported IFunctionValues.");
  }

  //if (compDomain)
  //{
  //}
  //else
  {
    addVal(m_domain,simpleValues);
  }

  // add penalty
  for(size_t i=0;i<m_function->nParams();++i)
  {
    if ( !m_function->isActive(i) ) continue;
    IConstraint* c = m_function->getConstraint(i);
    if (c)
    {
      m_value += c->check();
    }
  }

  m_dirtyVal = false;
  return m_value;
}

void LeastSquares::addVal(FunctionDomain_sptr domain, FunctionValues_sptr values)const
{
  m_function->function(*m_domain,*values);
  size_t ny = m_values->size();

  double retVal = 0.0;

  for (size_t i = 0; i < ny; i++)
  {
    double val = ( values->getCalculated(i) - values->getFitData(i) ) * values->getFitWeight(i);
    retVal += val * val;
  }
  
  m_value += 0.5 * retVal;

}


/// Calculate the derivatives of the cost function
/// @param der :: Container to output the derivatives
void LeastSquares::deriv(std::vector<double>& der) const
{
  valDerivHessian(false,true,false);

  if (der.size() != nParams())
  {
    der.resize(nParams());
  }
  for(size_t i = 0; i < nParams(); ++i)
  {
    der[i] = m_der.get(i);
  }
}

/// Calculate the value and the derivatives of the cost function
/// @param der :: Container to output the derivatives
/// @return :: The value of the function
double LeastSquares::valAndDeriv(std::vector<double>& der) const
{
  valDerivHessian(true,true,false);

  if (der.size() != nParams())
  {
    der.resize(nParams());
  }
  for(size_t i = 0; i < nParams(); ++i)
  {
    der[i] = m_der.get(i);
  }
  return m_value;
}

/** Calculate the value and the first and second derivatives of the cost function
 *  @param der :: Container to output the first derivatives
 *  @param hessian :: Container to output the second derivatives
 *  @param evalFunction :: If false cost function isn't evaluated and returned value (0.0) should be ignored.
 *    It is for efficiency reasons.
 *  @return :: The value of the function if evalFunction is true.
 */
double LeastSquares::valDerivHessian(bool evalFunction, bool evalDeriv, bool evalHessian) const
{
  if (m_pushed || !evalDeriv)
  {
    return val();
  }

  if (!m_dirtyVal && !m_dirtyDeriv && !m_dirtyHessian) return m_value;
  if (m_dirtyVal) evalFunction = true;

  checkValidity();

  if (evalFunction)
  {
    m_value = 0.0;
  }
  if (evalDeriv)
  {
    m_der.resize(nParams());
    m_der.zero();
  }
  if (evalHessian)
  {
    m_hessian.resize(nParams(),nParams());
    m_hessian.zero();
  }

  auto compDomain = boost::dynamic_pointer_cast<CompositeDomain>(m_domain);

  auto simpleValues = boost::dynamic_pointer_cast<FunctionValues>(m_values);
  if (!simpleValues)
  {
    throw std::runtime_error("LeastSquares: unsupported IFunctionValues.");
  }

  //if (compDomain)
  //{
  //}
  //else
  {
    addValDerivHessian(m_domain,simpleValues,evalFunction,evalDeriv,evalHessian);
  }

  // Add constraints penaly
  size_t np = m_function->nParams();
  if (evalFunction)
  {
    for(size_t i = 0; i < np; ++i)
    {
      IConstraint* c = m_function->getConstraint(i);
      if (c)
      {
        m_value += c->check();
      }
    }
    m_dirtyVal = false;
  }

  if (evalDeriv)
  {
    size_t i = 0;
    for(size_t ip = 0; ip < np; ++ip)
    {
      IConstraint* c = m_function->getConstraint(ip);
      if (c)
      {
        if ( !m_function->isActive(ip) ) continue;
        double d =  m_der.get(i) + c->checkDeriv();
        m_der.set(i,d);
      }
      ++i;
    }
    m_dirtyDeriv = false;
  }

  if (evalDeriv)
  {
    size_t i = 0;
    for(size_t ip = 0; ip < np; ++ip)
    {
      IConstraint* c = m_function->getConstraint(ip);
      if (c)
      {
        if ( !m_function->isActive(ip) ) continue;
        double d =  m_hessian.get(i,i) + c->checkDeriv2();
        m_hessian.set(i,i,d);
      }
      ++i;
    }
    m_dirtyHessian = false;
  }

  return m_value;
}

/**
 * Update the cost function, derivatives and hessian by adding values calculated
 * on a domain.
 */
void LeastSquares::addValDerivHessian(
  FunctionDomain_sptr domain,
  FunctionValues_sptr values,
  bool evalFunction , bool evalDeriv, bool evalHessian) const
{
  size_t np = m_function->nParams();  // number of parameters 
  size_t ny = m_domain->size(); // number of data points
  StdJacobian jacobian(ny,np);
  if (evalFunction)
  {
    m_function->function(*domain,*values);
  }
  m_function->functionDeriv(*domain,jacobian);

  size_t iActiveP = 0;
  double fVal = 0.0;
  if (debug)
  {
    std::cerr << "Jacobian:\n";
    for(size_t i = 0; i < ny; ++i)
    {
      for(size_t ip = 0; ip < np; ++ip)
      {
        if ( !m_function->isActive(ip) ) continue;
        std::cerr << jacobian.get(i,ip) << ' ';
      }
      std::cerr << std::endl;
    }
  }
  for(size_t ip = 0; ip < np; ++ip)
  {
    if ( !m_function->isActive(ip) ) continue;
    double d = m_der.get(iActiveP);
    for(size_t i = 0; i < ny; ++i)
    {
      double calc = values->getCalculated(i);
      double obs = values->getFitData(i);
      double w = values->getFitWeight(i);
      double y = ( calc - obs ) * w;
      d += y * jacobian.get(i,ip) * w;
      if (iActiveP == 0 && evalFunction)
      {
        fVal += y * y;
      }
    }
    m_der.set(iActiveP, d);
    //std::cerr << "der " << ip << ' ' << der[iActiveP] << std::endl;
    ++iActiveP;
  }

  if (evalFunction)
  {
    m_value += 0.5 * fVal;
  }

  if (!evalHessian) return;

  size_t na = m_der.size(); // number of active parameters

  size_t i1 = 0; // active parameter index
  for(size_t i = 0; i < np; ++i) // over parameters
  {
    if ( !m_function->isActive(i) ) continue;
    size_t i2 = 0; // active parameter index
    for(size_t j = 0; j <= i; ++j) // over ~ half of parameters
    {
      if ( !m_function->isActive(j) ) continue;
      double d = m_hessian.get(i1,i2);
      for(size_t k = 0; k < ny; ++k) // over fitting data
      {
        double w = values->getFitWeight(k);
        d += jacobian.get(k,i) * jacobian.get(k,j) * w * w;
      }
      m_hessian.set(i1,i2,d);
      //std::cerr << "hess " << i1 << ' ' << i2 << std::endl;
      if (i1 != i2)
      {
        m_hessian.set(i2,i1,d);
      }
      ++i2;
    }
    ++i1;
  }
}


const GSLVector& LeastSquares::getDeriv() const
{
  if (m_pushed)
  {
    return m_der;
  }
  if (m_dirtyVal || m_dirtyDeriv || m_dirtyHessian)
  {
    valDerivHessian();
  }
  return m_der;
}

const GSLMatrix& LeastSquares::getHessian() const
{
  if (m_pushed)
  {
    return m_hessian;
  }
  if (m_dirtyVal || m_dirtyDeriv || m_dirtyHessian)
  {
    valDerivHessian();
  }
  return m_hessian;
}

void LeastSquares::push()
{
  if (m_pushed)
  {
    throw std::runtime_error("Least squares: double push.");
  }
  // make sure we are not dirty
  m_pushedValue = valDerivHessian();
  getParameters(m_pushedParams);
  m_pushed = true;
}

void LeastSquares::pop()
{
  if ( !m_pushed )
  {
    throw std::runtime_error("Least squares: empty stack.");
  }
  setParameters(m_pushedParams);
  m_value = m_pushedValue;
  m_pushed = false;
  m_dirtyVal = false;
  m_dirtyDeriv = false;
  m_dirtyHessian = false;
}

void LeastSquares::drop()
{
  if ( !m_pushed )
  {
    throw std::runtime_error("Least squares: empty stack.");
  }
  m_pushed = false;
  setDirty();
}

void LeastSquares::setParameters(const GSLVector& params)
{
  if (nParams() != params.size())
  {
    throw std::runtime_error("Parameter vector has wrong size in LeastSquares.");
  }
  for(size_t i = 0; i < nParams(); ++i)
  {
    setParameter(i,params.get(i));
  }
  m_function->applyTies();
}

void LeastSquares::getParameters(GSLVector& params) const
{
  if (params.size() != nParams())
  {
    params.resize(nParams());
  }
  for(size_t i = 0; i < nParams(); ++i)
  {
    params.set(i,getParameter(i));
  }
}

/**
  * Calculates covariance matrix for fitting function's active parameters. 
  */
void LeastSquares::calActiveCovarianceMatrix(GSLMatrix& covar, double epsrel)
{
  if (m_hessian.isEmpty())
  {
    valDerivHessian();
  }
  covar = m_hessian;
  covar.invert();
}

} // Numeric
