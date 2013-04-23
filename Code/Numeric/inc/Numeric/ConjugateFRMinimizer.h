#ifndef NUMERIC_CONJUGATEFRMINIMIZER_H
#define NUMERIC_CONJUGATEFRMINIMIZER_H

#include "Numeric/DllExport.h"
#include "Numeric/Minimizer.h"

#include <gsl/gsl_multimin.h>

namespace Numeric
{

class NUMERIC_EXPORT ConjugateFRMinimizer: public Minimizer
{
public:
  ConjugateFRMinimizer();
  ~ConjugateFRMinimizer(){}
  virtual Result minimize();
private:
  static double fun(const gsl_vector * x, void * params);
  static void dfun(const gsl_vector * x, void * params, gsl_vector * g);
  static void fundfun (const gsl_vector * x, void * params, double * f, gsl_vector * g);
};

} // namespace Numeric
#endif // NUMERIC_CONJUGATEFRMINIMIZER_H
