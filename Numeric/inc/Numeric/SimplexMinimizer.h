#ifndef NUMERIC_SIMPLEXMINIMIZER_H
#define NUMERIC_SIMPLEXMINIMIZER_H

#include "Numeric/DllExport.h"
#include "Numeric/Minimizer.h"

#include <gsl/gsl_multimin.h>

namespace Numeric
{

class NUMERIC_EXPORT SimplexMinimizer: public Minimizer
{
public:
  SimplexMinimizer();
  ~SimplexMinimizer(){}
  virtual Result minimize();
private:
  static double fun(const gsl_vector * x, void * params);
};

} // namespace Numeric
#endif // NUMERIC_SIMPLEXMINIMIZER_H
