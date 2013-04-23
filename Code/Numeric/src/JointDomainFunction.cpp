#include "Numeric/JointDomainFunction.h"

namespace Numeric
{

/**
 * Constructor.
 */
JointDomainFunction::JointDomainFunction()
{
}

/**
 * Destructor.
 */
JointDomainFunction::~JointDomainFunction()
{
}

void JointDomainFunction::function(const FunctionDomain& domain, FunctionValues& values)const
{
}

/// Derivatives of function with respect to active parameters
void JointDomainFunction::functionDeriv(const FunctionDomain& domain, Jacobian& jacobian)
{
}


} // Numeric
