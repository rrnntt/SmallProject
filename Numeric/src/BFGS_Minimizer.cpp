//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Numeric/BFGS_Minimizer.h"
#include "Numeric/CostFunctionFactory.h"

namespace Numeric
{
//DECLARE_FUNCMINIMIZER(BFGS_Minimizer)

/// Return a concrete type to initialize m_gslSolver gsl_multimin_fdfminimizer_vector_bfgs2
const gsl_multimin_fdfminimizer_type* BFGS_Minimizer::getGSLMinimizerType()
{
  return gsl_multimin_fdfminimizer_vector_bfgs2;
}

} // Numeric
