#include "Numeric/ConjugateFRMinimizer.h"

namespace Numeric
{

/** Fit GSL function wrapper
* @param x :: Input function parameters
* @param params :: Pointer to SimplexMinimizer
* @return The result
*/
double ConjugateFRMinimizer::fun (const gsl_vector * x, void * params)
{
  ConjugateFRMinimizer& minimizer = *static_cast<ConjugateFRMinimizer*>(params);
  size_t n = minimizer.m_costFunction->nParams();
  for(size_t i = 0;i < n; ++i)
  {
    minimizer.m_costFunction->setParameter(i,gsl_vector_get(x, i));
  }
  return minimizer.m_costFunction->eval();
}

void ConjugateFRMinimizer::dfun(const gsl_vector * x, void * params, gsl_vector * g)
{
  ConjugateFRMinimizer& minimizer = *static_cast<ConjugateFRMinimizer*>(params);
  size_t n = minimizer.m_costFunction->nParams();
  for(size_t i = 0;i < n; ++i)
  {
    minimizer.m_costFunction->setParameter(i,gsl_vector_get(x, i));
  }
  std::vector<double> der(n);
  minimizer.m_costFunction->deriv(der);
  for(size_t i = 0;i < n; ++i)
  {
    gsl_vector_set(g, i, der[i]);
  }
}

void ConjugateFRMinimizer::fundfun (const gsl_vector * x, void * params, double * f, gsl_vector * g)
{
  ConjugateFRMinimizer& minimizer = *static_cast<ConjugateFRMinimizer*>(params);
  size_t n = minimizer.m_costFunction->nParams();
  for(size_t i = 0;i < n; ++i)
  {
    minimizer.m_costFunction->setParameter(i,gsl_vector_get(x, i));
  }
  std::vector<double> der(n);
  *f = minimizer.m_costFunction->evalAndDeriv(der);
  for(size_t i = 0;i < n; ++i)
  {
    gsl_vector_set(g, i, der[i]);
  }
}

ConjugateFRMinimizer::ConjugateFRMinimizer()
{
}

Minimizer::Result ConjugateFRMinimizer::minimize()
{
  gsl_multimin_function_fdf minex_func;

  size_t nParams = m_costFunction->nParams();
  minex_func.n = nParams;
  minex_func.f = &ConjugateFRMinimizer::fun;
  minex_func.df = &ConjugateFRMinimizer::dfun;
  minex_func.fdf = &ConjugateFRMinimizer::fundfun;
  minex_func.params = this;

  gsl_vector *x;

  // Starting point 
  x = gsl_vector_alloc (nParams);
  for(size_t i = 0; i < nParams; ++i)
  {
    gsl_vector_set (x, i, m_costFunction->getParameter(i));
  }

  gsl_multimin_fdfminimizer *s = gsl_multimin_fdfminimizer_alloc (gsl_multimin_fdfminimizer_conjugate_fr, nParams);
  gsl_multimin_fdfminimizer_set (s, &minex_func, x, 0.01, 1e-4);

  size_t iter = 0;
  int status;
  do
  {
    iter++;
    status = gsl_multimin_fdfminimizer_iterate(s);

    std::cerr << "iter " << iter << ' ' << m_costFunction->eval() << ' ' << status << std::endl;

    if (status) 
      break;

    status = gsl_multimin_test_gradient (s->gradient, 1e-3); //! <---------------

  }
  while (status == GSL_CONTINUE && iter < 100); //! <---------------

  gsl_vector_free(x);
  gsl_multimin_fdfminimizer_free (s);

  return (status == GSL_SUCCESS) ? Success : Failed;
}

} // namespace Numeric
