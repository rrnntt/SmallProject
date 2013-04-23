#include "Numeric/ConstantFunction.h"
#include "Numeric/FunctionFactory.h"

#include <algorithm>

namespace Numeric
{

DECLARE_FUNCTION(ConstantFunction);

/**
 * Constructor.
 */
ConstantFunction::ConstantFunction()
{
    declareParameter("C");
}

void ConstantFunction::function1D(double *out, const double *xValues, const size_t nData) const
{
    const double c = getParameter(0);
    std::fill(out, out + nData, c);
}

void ConstantFunction::functionDeriv1D(Jacobian *out, const double *xValues, const size_t nData)
{
    for(size_t i = 0; i < nData; ++i)
    {
        out->set(i, 0, 1.0);
    }
}

} // Numeric
