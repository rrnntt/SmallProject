#ifndef NUMERIC_LEVENBERGMARQUARDT_H
#define NUMERIC_LEVENBERGMARQUARDT_H

#include "Numeric/DllExport.h"
#include "Numeric/IFuncMinimizer.h"
#include "Numeric/GSLVector.h"
#include "Numeric/GSLMatrix.h"

namespace Numeric
{

class LeastSquares;

class NUMERIC_EXPORT LevenbergMarquardt: public IFuncMinimizer
{
public:
  /// Constructor
  LevenbergMarquardt();
  /// Name of the minimizer.
  std::string name() const {return "LevenbergMarquardt";}

  /// Initialize minimizer, i.e. pass a function to minimize.
  virtual void initialize(ICostFunction_sptr function);
  /// Do one iteration.
  virtual bool iterate();
  /// Return current value of the cost function
  virtual double costFunctionVal();

private:
  /// Pointer to the cost function. Must be the least squares.
  boost::shared_ptr<LeastSquares> m_leastSquares;
  /// Relative tolerance.
  double m_relTol;
  /// The tau parameter in the Levenberg-Marquardt method.
  double m_tau;
  /// The damping mu parameter in the Levenberg-Marquardt method.
  double m_mu;
  /// The nu parameter in the Levenberg-Marquardt method.
  double m_nu;
  /// The rho parameter in the Levenberg-Marquardt method.
  double m_rho;
  /// To keep function value
  double m_F;
  std::vector<double> m_D;
};

} // NUMERIC

#endif // NUMERIC_LEVENBERGMARQUARDT_H
