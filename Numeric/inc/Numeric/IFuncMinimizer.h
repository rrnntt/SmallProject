#ifndef NUMERIC_IFUNCMINIMIZER_H
#define NUMERIC_IFUNCMINIMIZER_H

#include "Numeric/DllExport.h"
#include "Numeric/ICostFunction.h"

#include <string>

namespace Numeric
{

class NUMERIC_EXPORT IFuncMinimizer
{
public:
  /// Virtual destructor
  virtual ~IFuncMinimizer() {}

  /// Initialize minimizer, i.e. pass function and costFunction
  virtual void initialize(ICostFunction_sptr function) = 0;

  /// Get name of minimizer
  virtual std::string name() const = 0;

  /// Do one iteration
  /// @return :: true if iterations should be continued or false to stop
  virtual bool iterate() = 0;

  /// Perform iteration with minimizer and return info about how well this went
  /// using the GSL status integer system. See gsl_errno.h for details.
  virtual bool minimize(size_t maxIterations = 1000);

  virtual std::string getError() const {return m_errorString;}

  /// Get value of cost function 
  virtual double costFunctionVal() = 0;

protected:
  std::string m_errorString;
};

typedef boost::shared_ptr<IFuncMinimizer> IFuncMinimizer_sptr;

} // NUMERIC

#endif // NUMERIC_IFUNCMINIMIZER_H
