#include "Numeric/DVRFunction.h"

namespace Numeric
{

/**
 * Constructor.
 */
DVRFunction::DVRFunction(const DVRFunctionBase_sptr &base):
    m_base(base)
{
    if ( !base )
    {
        throw std::runtime_error("Cannot create DVRFunction with empty base.");
    }
    m_data.resize( base->size() );
}

/**
 * Copy constructor. Copies (shares) the function base only.
 * @param fun
 */
DVRFunction::DVRFunction(const DVRFunction &fun):
    m_base(fun.m_base)
{
    m_data.resize( m_base->size() );
}

/**
 * Destructor.
 */
DVRFunction::~DVRFunction()
{
}

} // Numeric
