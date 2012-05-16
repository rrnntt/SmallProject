#include "Numeric/ChebOperator.h"

namespace Numeric
{

//--------------------------------------------------------
/**
 * Constructor
 * @param val :: Initial function value
 * @param der :: Initial function derivative
 */
Cauchy::Cauchy(double val, double der):
m_value(val),
m_deriv(der)
{
}

/**
 * Apply the conditions to an equation L.y = rhs
 * @param L :: Matrix of a differential operator.
 * @param rhs :: The right-hand-side vector of the equation
 */
void Cauchy::apply(ChebfunBase_const_sptr base, GSLMatrix& L, GSLVector& rhs) const
{
  const size_t n = L.size1();
  assert( n == L.size2() );
  assert( n == rhs.size() );
  double l00 = L.get(0, 0);
  L.set(0, 0, l00 + 1.0);
  rhs.set(0, m_value);
  GSLMatrix D;
  ChebDiff diff;
  diff.createMatrix( base, D );
  assert( n == D.size1() );
  for(size_t i = 0; i < n; ++i)
  {
    double t = L.get(1, i);
    t += D.get(0, i);
    L.set(1, i, t);
  }
  rhs.set(1, m_deriv);
}

//--------------------------------------------------------
/**
 * Constructor
 * @param val :: Function value at the start of domain
 */
Dirichlet::Dirichlet(double val):
m_leftValue(val),
m_rightValue(0),
m_hasLeft(true),
m_hasRight(false)
{
}

/**
 * Constructor
 * @param val1 :: Function value at the start of domain
 * @param val2 :: Function value at the end of domain
 */
Dirichlet::Dirichlet(double val1, double val2):
m_leftValue(val1),
m_rightValue(val2),
m_hasLeft(true),
m_hasRight(true)
{
}

/**
 * Apply the conditions to an equation L.y = rhs
 * @param L :: Matrix of a differential operator.
 * @param rhs :: The right-hand-side vector of the equation
 */
void Dirichlet::apply(ChebfunBase_const_sptr base, GSLMatrix& L, GSLVector& rhs) const
{
  const size_t n = L.size1();
  assert( n == L.size2() );
  assert( n == rhs.size() );
  if ( m_hasLeft )
  {
    double tmp = L.get(0, 0);
    L.set(0, 0, tmp + 1.0);
    rhs.set(0, m_leftValue);
  }
  if ( m_hasRight )
  {
    double tmp = L.get(n - 1, n - 1);
    L.set(n - 1, n - 1, tmp + 1.0);
    rhs.set(n - 1, m_leftValue);
  }
}

//--------------------------------------------------------
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

/**
 * Solve the equation L.y = 0 where L is the matrix of this operator
 * @param y :: The solution
 * @param bc :: The boundary conditions
 */
void ChebOperator::solve(chebfun& y, const BoundaryConditions& bc)
{
  GSLMatrix L;
  size_t n = y.n() + 1;
  createMatrix( y.getBase(), L );

  GSLVector rhs(n);
  rhs.zero();

  bc.apply(y.getBase(), L, rhs);
  GSLVector u(n);
  L.solve(rhs, u);

  y.setP( u );
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

/**
 * Add an operator from the right
 * @param op :: Operator to add. ChebCompositeOperator takes owneship of the 
 * new operator.
 */
void ChebCompositeOperator::addRight(ChebOperator* op)
{
  m_operators.push_back( op );
}

/**
 * Add an operator from the left
 * @param op :: Operator to add. ChebCompositeOperator takes owneship of the 
 * new operator.
 */
void ChebCompositeOperator::addLeft(ChebOperator* op)
{
  m_operators.push_front( op );
}

/**
 * Create operator matrix
 * @param base :: The base of the result function
 * @param L :: The output matrix
 */
void ChebCompositeOperator::createMatrix(ChebfunBase_const_sptr base, GSLMatrix& L)
{
  const size_t n = base->n + 1;
  L.resize(n, n);
  if ( m_operators.empty() )
  {
    L.identity();
    return;
  }
  auto op = m_operators.begin();
  (**op).createMatrix( base, L );
  ++op;
  for(; op != m_operators.end(); ++op)
  {
    GSLMatrix M(n,n);
    (**op).createMatrix( base, M );
    GSLMatrix R(n,n);
    R = L * M;
    L.swap( R );
  }
}

//--------------------------------------------------------

/**
 * Add another operator
 * @param op :: '+' or '-'
 * @param oper :: Operator to add. ChebCompositeOperator takes owneship of the 
 * new operator.
 */
void ChebPlus::add(char op, ChebOperator* oper)
{
  addRight( oper );
  m_ops.push_back( op );
}

/**
 * Create operator matrix
 * @param base :: The base of the result function
 * @param L :: The output matrix
 */
void ChebPlus::createMatrix(ChebfunBase_const_sptr base, GSLMatrix& L)
{
  const size_t n = base->n + 1;
  L.resize(n, n);
  L.zero();
  size_t i = 0;
  for(auto op = m_operators.begin(); op != m_operators.end(); ++op, ++i)
  {
    GSLMatrix M(n,n);
    (**op).createMatrix( base, M );
    if ( m_ops[i] == '+' )
    {
      L += M;
    }
    else
    {
      L -= M;
    }
  }
}

//--------------------------------------------------------

/// Constructor
ChebTimes::ChebTimes(double factor):
m_constant(factor),
m_isFun(false)
{
}

/// Constructor
ChebTimes::ChebTimes(const chebfun& fun):
m_constant(0),
m_fun(fun),
m_isFun(true)
{
}

/**
 * Create operator matrix
 * @param base :: The base of the result function
 * @param L :: The output matrix
 */
void ChebTimes::createMatrix(ChebfunBase_const_sptr base, GSLMatrix& L)
{
  const size_t n = base->n + 1;
  L.resize(n, n);
  L.zero();
  auto& x = base->x;
  if ( m_isFun )
  {
    for( size_t i = 0; i < n; ++i )
    {
      L.set( i, i, m_fun(x[i]) );
    }
  }
  else
  {
    for( size_t i = 0; i < n; ++i )
    {
      L.set( i, i, m_constant );
    }
  }
}

//--------------------------------------------------------

} // Numeric
