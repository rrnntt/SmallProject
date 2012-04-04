#ifndef NUMERIC_COSTFUNCFITTING_H
#define NUMERIC_COSTFUNCFITTING_H

#include "Numeric/DllExport.h"
#include "Numeric/ICostFunction.h"
#include "Numeric/IFunction.h"
#include "Numeric/GSLMatrix.h"

namespace Numeric
{

class NUMERIC_EXPORT CostFuncFitting : public ICostFunction 
{
public:
  CostFuncFitting();
  /// Get i-th parameter
  /// @param i :: Index of a parameter
  /// @return :: Value of the parameter
  virtual double getParameter(size_t i)const;
  /// Set i-th parameter
  /// @param i :: Index of a parameter
  /// @param value :: New value of the parameter
  virtual void setParameter(size_t i, const double& value);
  /// Number of parameters
  virtual size_t nParams()const;

  /// Set fitting function.
  virtual void setFittingFunction(IFunction_sptr function, 
    FunctionDomain_sptr domain, IFunctionValues_sptr values);

  /// Get fitting function.
  virtual IFunction_sptr getFittingFunction(){return m_function;}

  /// Calculates covariance matrix
  /// @param covar :: Returned covariance matrix, here as 
  /// @param epsrel :: Is used to remove linear-dependent columns
  ///
  virtual void calCovarianceMatrix(GSLMatrix& covar, double epsrel = 1e-8);

  /// Calculate fitting errors
  virtual void calFittingErrors(const GSLMatrix& covar);
  FunctionDomain_sptr getDomain() const {return m_domain;}
  IFunctionValues_sptr getValues() const {return m_values;}

protected:

  /**
   * Calculates covariance matrix for fitting function's active parameters. 
   */
  virtual void calActiveCovarianceMatrix(GSLMatrix& covar, double epsrel = 1e-8);

  bool isValid() const;
  void checkValidity() const;
  void calTransformationMatrixNumerically(GSLMatrix& tm);
  void setDirty();
  
  IFunction_sptr m_function;
  FunctionDomain_sptr m_domain;
  IFunctionValues_sptr m_values;
  std::vector<size_t> m_indexMap;

  mutable bool m_dirtyVal;
  mutable bool m_dirtyDeriv;
  mutable bool m_dirtyHessian;
};

} // NUMERIC

#endif // NUMERIC_COSTFUNCFITTING_H
