#include "Numeric/ChebOperator.h"

namespace Numeric
{

/**
 * Constructor.
 */
ChebOperator::ChebOperator()
{
}

/**
 * Destructor.
 */
ChebOperator::~ChebOperator()
{
  for(auto it = m_matrixCache.begin(); it != m_matrixCache.end(); ++it)
  {
    if ( it->second ) delete it->second;
  }
}

/** Apply this operator to a chebfun.
 * @param arg :: Function to apply the operator to.
 * @param res :: Function - result of the operation.
 */
void ChebOperator::apply(const chebfun& arg, chebfun& res)
{

  auto base = res.getBase();
  GSLMatrix* L;
  if ( m_matrixCache.count( base ) != 0 )
  {
    L = m_matrixCache[base];
  }
  else
  {
    L = new GSLMatrix;
    createMatrix( base, *L );
    m_matrixCache[base] = L;
  }
  std::vector<double> xy;
  if ( arg.haveSameBase( res ) )
  {
    xy = arg.ypoints();
  }
  else
  {
    auto& rx = res.xpoints();
    xy.resize( rx.size() );
    for(size_t i = 0; i < rx.size(); ++i)
    {
      xy[i] = arg( rx[i] );
    }
  }
  GSLVector x( xy );
  GSLVector y;
  L->multiply( x, y );
  res.setP( y );
}


//--------------------------------------------------------
/** Create operator matrix.
 *  @param base :: The base of the result function
 */
void ChebDiff::createMatrix(ChebfunBase_const_sptr base, GSLMatrix& L)
{
  const size_t n = base->n + 1;
  L.resize(n, n);
  auto& x = base->x;
  for(size_t i = 0; i < n; ++i)
  {
    const double& xi = x[i];
    std::vector<double> dx( n );
    double Lii = 0.0; // diagonal ME
    for(size_t j = 0; j < n; ++j)
    {
      if ( j == i ) continue;
      const double& xj = x[j];
      double xij = 1.0 / (xj - xi);
      Lii -= xij;
      double Lij = xij; // non-diagonal ME
      for(size_t k = 0; k < n; ++k)
      {
        if ( k == j || k == i ) continue;
        const double& xk = x[k];
        Lij *= ( xi - xk ) / ( xj - xk );
      }
      L.set( i, j, Lij );
      //std::cerr << i << ' ' << j << ' ' << Lij << std::endl;
    }
    L.set( i, i, Lii );
  }
}

//--------------------------------------------------------

/** Create operator matrix.
 *  @param base :: The base of the result function
 */
void ChebIdentity::createMatrix(ChebfunBase_const_sptr base, GSLMatrix& L)
{
  const size_t n = base->n + 1;
  L.resize(n, n);
  L.identity();
}

//--------------------------------------------------------

/**
 * Destructor.
 */
ChebCompositeOperator::~ChebCompositeOperator()
{
  for(auto it = m_operators.begin(); it != m_operators.end(); ++it)
  {
    delete *it;
  }
}


} // Numeric
