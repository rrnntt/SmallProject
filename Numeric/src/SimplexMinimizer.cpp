#include "Numeric/SimplexMinimizer.h"

namespace Numeric
{

/** Fit GSL function wrapper
* @param x :: Input function parameters
* @param params :: Pointer to SimplexMinimizer
* @return The result
*/
double SimplexMinimizer::fun (const gsl_vector * x, void * params)
{
  SimplexMinimizer& simplex = *static_cast<SimplexMinimizer*>(params);
  size_t n = simplex.m_costFunction->nParams();
  for(size_t i = 0;i < n; ++i)
  {
    simplex.m_costFunction->setParameter(i,gsl_vector_get(x, i));
  }
  return simplex.m_costFunction->eval();
}

SimplexMinimizer::SimplexMinimizer()
{
}

Minimizer::Result SimplexMinimizer::minimize()
{
  gsl_multimin_function minex_func;

  size_t nParams = m_costFunction->nParams();
  minex_func.n = nParams;
  minex_func.f = &SimplexMinimizer::fun;
  minex_func.params = this;

  gsl_vector *ss, *x;

  // Starting point 
  x = gsl_vector_alloc (nParams);
  for(size_t i = 0; i < nParams; ++i)
  {
    gsl_vector_set (x, i, m_costFunction->getParameter(i));
  }

  // Set initial step sizes to 1
  ss = gsl_vector_alloc (nParams);
  gsl_vector_set_all (ss, 0.10);

  gsl_multimin_fminimizer *s = gsl_multimin_fminimizer_alloc (gsl_multimin_fminimizer_nmsimplex, nParams);
  gsl_multimin_fminimizer_set (s, &minex_func, x, ss);

  size_t iter = 0;
  int status;
  double size;
  do
  {
    iter++;
    status = gsl_multimin_fminimizer_iterate(s);

    std::cerr << "iter " << iter << ' ' << m_costFunction->eval() << std::endl;

    if (status) 
      break;

    size = gsl_multimin_fminimizer_size (s);
    status = gsl_multimin_test_size (size, 1e-5); //! <---------------

  }
  while (status == GSL_CONTINUE && iter < 100); //! <---------------

  gsl_vector_free(x);
  gsl_vector_free(ss);
  gsl_multimin_fminimizer_free (s);

  return (status == GSL_SUCCESS) ? Success : Failed;
}

} // namespace Numeric
