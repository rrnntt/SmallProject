#include "Numeric/DVRFunctionBase.h"

namespace Numeric
{

/**
 * Constructor.
 */
DVRFunctionBase::DVRFunctionBase(FunctionDomainMD *domain):
    m_domain(domain)
{
    if ( !domain )
    {
        throw std::runtime_error("Domain musn't be null in DRVFunctionBase.");
    }
}

/**
 * Destructor.
 */
DVRFunctionBase::~DVRFunctionBase()
{
    delete m_domain;
}

} // Numeric
