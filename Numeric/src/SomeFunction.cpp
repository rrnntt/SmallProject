#include "Numeric/SomeFunction.h"

#include <gsl/gsl_vector.h>

double SomeFunction()
{
  gsl_vector *v = gsl_vector_alloc(3);
  gsl_vector_set(v,0,1.0);
  gsl_vector_set(v,1,2.0);
  gsl_vector_set(v,2,3.0);

  double res = gsl_vector_get(v,2);
  gsl_vector_free(v);
  return res;
}
