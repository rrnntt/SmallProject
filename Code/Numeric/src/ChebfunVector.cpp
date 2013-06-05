#include "Numeric/ChebfunVector.h"
#include "API/WorkspaceFactory.h"

#include <sstream>

namespace Numeric
{

DECLARE_WORKSPACE(ChebfunVector)

/**
 * Constructor.
 */
ChebfunVector::ChebfunVector()
{
}

/**
 * Destructor.
 */
ChebfunVector::~ChebfunVector()
{
}

//--------------------------------------------------------------------------------------------
/**
 * Create a ChebfunVector (this) from another ChebfunVector as their linear combinations.
 * @param basis :: The basis functions for the linear combinations.
 * @param coeffs :: Coefficients for the combinations.
 */
void ChebfunVector::fromLinearCombinations(const ChebfunVector& basis, const GSLMatrix& coeffs)
{
  const size_t n1 = coeffs.size1();
  const size_t n2 = coeffs.size2();
  if ( n2 > basis.size() )
  {
    std::ostringstream ss;
    ss << "Sizes of the basis and the coefficient matrix are not the same.\n" << n1 << ' ' << n2 << ' ' << basis.size() << std::endl;
    throw std::invalid_argument(ss.str());
  }

  // assuming here that all functions in basis have the same chebfun base
  const ChebFunction& b = basis.cfun(0);
  for(size_t i = 0; i < n1; ++i)
  {
    ChebFunction_sptr f( new ChebFunction(b,false) );
    for(size_t j = 0; j < n2; ++j)
    {
      ChebFunction tmp( basis.cfun(j) );
      tmp *= coeffs.get(j,i);
      *f += tmp;
    }
    add( f );
  }
}

//--------------------------------------------------------------------------------------------
/// Sort the functions in the vector according to a table
void ChebfunVector::sort(const std::vector<size_t>& indx)
{
  if ( indx.size() != size() )
  {
    throw std::invalid_argument("Index vector has a wrong size.");
  }
  std::vector<ChebFunction_sptr> tmp;
  tmp.assign(m_funs.begin(), m_funs.end());
  for(size_t i = 0; i < indx.size(); ++i)
  {
    m_funs[i] = tmp[ indx[i] ];
  }

}

} // Numeric
