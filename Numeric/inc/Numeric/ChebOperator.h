#ifndef NUMERIC_CHEBOPERATOR_H
#define NUMERIC_CHEBOPERATOR_H

#include "Numeric/DllExport.h"
#include "Numeric/ChebFunction.h"
#include "Numeric/GSLMatrix.h"

#include <map>
#include <list>

namespace Numeric
{

/**
 * Base class for boundary conditions for solving ordinary differential equations.
 */
class NUMERIC_EXPORT BoundaryConditions
{
public:
  virtual ~BoundaryConditions() {}
  /// Apply the conditions to an equation
  virtual void apply(ChebfunBase_const_sptr base, GSLMatrix& L, GSLVector& rhs) const = 0;
};

/**
 * Cauchy boundary conditions: boundary value + first derivative
 */
class NUMERIC_EXPORT Cauchy: public BoundaryConditions
{
public:
  /// Constructor
  Cauchy(double val, double der);
  /// Apply the conditions to an equation
  virtual void apply(ChebfunBase_const_sptr base, GSLMatrix& L, GSLVector& rhs) const;
protected:
  double m_value;
  double m_deriv;
};

/**
 * Cauchy boundary conditions: boundary value + first derivative
 */
class NUMERIC_EXPORT Dirichlet: public BoundaryConditions
{
public:
  /// Constructor
  Dirichlet(double val);
  /// Constructor
  Dirichlet(double val1, double val2);
  /// Apply the conditions to an equation
  virtual void apply(ChebfunBase_const_sptr base, GSLMatrix& L, GSLVector& rhs) const;
protected:
  double m_leftValue;
  double m_rightValue;
  bool m_hasLeft;
  bool m_hasRight;
};

/**
 * A base class for a linear operator acting on a chebfun.
 */
class NUMERIC_EXPORT ChebOperator
{
public:
  ChebOperator();
  virtual ~ChebOperator();
  /// Apply this operator to a chebfun.
  /// @param arg :: Function to apply the operator to.
  /// @param res :: Function - result of the operation.
  virtual void apply(const chebfun& arg, chebfun& res);
  /// Create operator matrix
  /// @param base :: The base of the result function
  virtual void createMatrix(ChebfunBase_const_sptr base, GSLMatrix& L) = 0;
  /// Solve the equation L.y = 0 where L is the matrix of this operator
  void solve(chebfun& y, const BoundaryConditions& bc);
protected:
  /// Cached operator matrices for different chebfun bases
  std::map< ChebfunBase_const_sptr, GSLMatrix* > m_matrixCache;
};

/**
 * A first derivative operator.
 */
class NUMERIC_EXPORT ChebDiff: public ChebOperator
{
public:
  /// Create operator matrix
  /// @param base :: The base of the result function
  void createMatrix(ChebfunBase_const_sptr base, GSLMatrix& L);
};

/**
 * The identity operator.
 */
class NUMERIC_EXPORT ChebIdentity: public ChebOperator
{
public:
  /// Create operator matrix
  /// @param base :: The base of the result function
  void createMatrix(ChebfunBase_const_sptr base, GSLMatrix& L);
};

/**
 * A composite operator.
 */
class NUMERIC_EXPORT ChebCompositeOperator: public ChebOperator
{
public:
  ~ChebCompositeOperator();
  /// Add an operator from the right
  void addRight(ChebOperator* op);
  /// Add an operator from the left
  void addLeft(ChebOperator* op);
  /// Create operator matrix
  /// @param base :: The base of the result function
  /// @param L :: The output matrix
  void createMatrix(ChebfunBase_const_sptr base, GSLMatrix& L);
protected:
  std::list<ChebOperator*> m_operators;
};

/**
 * A plus operator.
 */
class NUMERIC_EXPORT ChebPlus: public ChebCompositeOperator
{
public:
  /// Add another operator
  void add(char op, ChebOperator* oper);
  /// Create operator matrix
  /// @param base :: The base of the result function
  /// @param L :: The output matrix
  void createMatrix(ChebfunBase_const_sptr base, GSLMatrix& L);
protected:
  using ChebCompositeOperator::addRight;
  using ChebCompositeOperator::addLeft;
  std::vector<char> m_ops;
};

/**
 * A second derivative operator
 */
class NUMERIC_EXPORT ChebDiff2: public ChebCompositeOperator
{
public:
  ChebDiff2()
  {
    addRight( new ChebDiff );
    addRight( new ChebDiff );
  }
};

/**
 * The multiplication by a function operator.
 */
class NUMERIC_EXPORT ChebTimes: public ChebOperator
{
public:
  /// Constructor
  ChebTimes(double factor);
  /// Constructor
  ChebTimes(const chebfun& fun);
  /// Create operator matrix
  /// @param base :: The base of the result function
  void createMatrix(ChebfunBase_const_sptr base, GSLMatrix& L);
protected:
  /// the constant
  double m_constant;
  /// the function
  chebfun m_fun;
  /// switch between the constant and the function
  bool m_isFun;
};

} // NUMERIC

#endif // NUMERIC_CHEBOPERATOR_H
