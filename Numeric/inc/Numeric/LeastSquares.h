#ifndef NUMERIC_LEASTSQUARES_H
#define NUMERIC_LEASTSQUARES_H

#include "Numeric/DllExport.h"
#include "Numeric/CostFuncFitting.h"
#include "Numeric/FunctionDomain.h"
#include "Numeric/GSLMatrix.h"
#include "Numeric/GSLVector.h"

namespace Numeric
{

class NUMERIC_EXPORT LeastSquares: public CostFuncFitting
{
public:
  /// Constructor
  LeastSquares():CostFuncFitting(),m_value(0),m_pushed(false){}
  /// Virtual destructor
  virtual ~LeastSquares() {}

  /// Get name of minimizer
  virtual std::string name() const { return "Least squares";}

  /// Get short name of minimizer - useful for say labels in guis
  virtual std::string shortName() const {return "Chi-sq";};

  /// Calculate value of cost function
  virtual double val() const;

  /// Calculate the derivatives of the cost function
  /// @param der :: Container to output the derivatives
  virtual void deriv(std::vector<double>& der) const;

  /// Calculate the value and the derivatives of the cost function
  /// @param der :: Container to output the derivatives
  /// @return :: The value of the function
  virtual double valAndDeriv(std::vector<double>& der) const;

  virtual double valDerivHessian(bool evalFunction = true, bool evalDeriv = true, bool evalHessian = true) const;
  const GSLVector& getDeriv() const;
  const GSLMatrix& getHessian() const;
  void push();
  void pop();
  void drop();

  void setParameters(const GSLVector& params);
  void getParameters(GSLVector& params) const;

protected:

  virtual void calActiveCovarianceMatrix(GSLMatrix& covar, double epsrel = 1e-8);

  void addVal(
    FunctionDomain_sptr domain,
    FunctionValues_sptr values
    )const;
  void addValDerivHessian(
    FunctionDomain_sptr domain,
    FunctionValues_sptr values,
    bool evalFunction = true, bool evalDeriv = true, bool evalHessian = true) const;

private:

  mutable double m_value;
  mutable GSLVector m_der;
  mutable GSLMatrix m_hessian;

  mutable bool m_pushed;
  mutable double m_pushedValue;
  mutable GSLVector m_pushedParams;

};

} // NUMERIC

#endif // NUMERIC_LEASTSQUARES_H
