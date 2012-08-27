#ifndef NUMERIC_CHEBOPERATOR_H
#define NUMERIC_CHEBOPERATOR_H

#include "Numeric/DllExport.h"
#include "Numeric/Chebfun.h"
#include "Numeric/ScaledChebfun.h"
#include "Numeric/GSLMatrix.h"
#include "Numeric/IFunction1D.h"

#include <map>
#include <list>
#include <string>

namespace Kernel
{
  class EParser;
}

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
 * Dirichlet boundary conditions: values on the left or both boundaries
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
 * Dirichlet boundary conditions: values on the left or both boundaries
 */
class NUMERIC_EXPORT ThroughPoint: public BoundaryConditions
{
public:
  /// Constructor
  ThroughPoint(double x, double y);
  /// Apply the conditions to an equation
  virtual void apply(ChebfunBase_const_sptr base, GSLMatrix& L, GSLVector& rhs) const;
protected:
  double m_x;
  double m_y;
};

/**
 * A base class for a linear operator acting on a chebfun.
 */
class NUMERIC_EXPORT ChebOperator
{
public:
  ChebOperator();
  virtual ~ChebOperator();
  /// Create operator matrix for an normal chebfun.
  /// @param base :: The base of the result function
  virtual void createMatrix(ChebfunBase_const_sptr base, GSLMatrix& L) = 0;
  /// Create operator matrix for a scaled chebfun.
  /// @param fun :: The result function
  virtual void createMatrix(const ScaledChebfun& fun, GSLMatrix& L) = 0;
  /// Apply this operator to a chebfun.
  /// @param arg :: Function to apply the operator to.
  /// @param res :: Function - result of the operation.
  virtual void apply(const chebfun& arg, chebfun& res);
  /// Apply this operator to a scaled chebfun.
  /// @param arg :: Function to apply the operator to.
  /// @param res :: Function - result of the operation.
  virtual void apply(const ScaledChebfun& arg, ScaledChebfun& res);
  /// Solve the equation L.y = 0 where L is the matrix of this operator
  void solve(chebfun& y, const BoundaryConditions& bc);
  /// Solve the equation L.y = 0 where L is the matrix of this operator
  void solve(ScaledChebfun& y, const BoundaryConditions& bc);
  /// Create an operator from a string.
  static ChebOperator* create( const std::string& str );
  /// Print out the operator for debugging
  /// @param padding :: Padding with spaces that must start all new lines in the log
  virtual void log(const std::string& padding = "") = 0;
protected:
  /// Create an operator from a EParser.
  static ChebOperator* create( const Kernel::EParser& parser );
  /// Get access to the plain chebfun of a ScaledChebfun
  const chebfun& getPlainChebfun(const ScaledChebfun& fun) const {return fun.m_fun;}
  /// Get access to protected ScaledChebfun::transform method
  double getScaledTransform(const ScaledChebfun& fun, double x) const {return fun.transform(x);}
  /// Get access to protected ScaledChebfun::invTransform method
  double getScaledInvTransform(const ScaledChebfun& fun, double x) const {return fun.invTransform(x);}
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
  /// Create operator matrix for a scaled chebfun.
  /// @param fun :: The result function
  void createMatrix(const ScaledChebfun& fun, GSLMatrix& L);
  /// Print out the operator for debugging
  /// @param padding :: Padding with spaces that must start all new lines in the log
  virtual void log(const std::string& padding = "");
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
  /// Create operator matrix for a scaled chebfun.
  /// @param fun :: The result function
  void createMatrix(const ScaledChebfun& fun, GSLMatrix& L);
  /// Print out the operator for debugging
  /// @param padding :: Padding with spaces that must start all new lines in the log
  virtual void log(const std::string& padding = "");
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
  /// Create operator matrix for a scaled chebfun.
  /// @param fun :: The result function
  void createMatrix(const ScaledChebfun& fun, GSLMatrix& L);
  /// Print out the operator for debugging
  /// @param padding :: Padding with spaces that must start all new lines in the log
  virtual void log(const std::string& padding = "");
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
  /// Create operator matrix for a scaled chebfun.
  /// @param fun :: The result function
  void createMatrix(const ScaledChebfun& fun, GSLMatrix& L);
  /// Print out the operator for debugging
  /// @param padding :: Padding with spaces that must start all new lines in the log
  virtual void log(const std::string& padding = "");
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
  /// Print out the operator for debugging
  /// @param padding :: Padding with spaces that must start all new lines in the log
  virtual void log(const std::string& padding = "");
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
  ChebTimes(IFunction1D_sptr fun);
  /// Create operator matrix
  /// @param base :: The base of the result function
  void createMatrix(ChebfunBase_const_sptr base, GSLMatrix& L);
  /// Create operator matrix for a scaled chebfun.
  /// @param fun :: The result function
  void createMatrix(const ScaledChebfun& fun, GSLMatrix& L);
  /// Print out the operator for debugging
  /// @param padding :: Padding with spaces that must start all new lines in the log
  virtual void log(const std::string& padding = "");
protected:
  /// the constant
  double m_constant;
  /// the function
  IFunction1D_sptr m_fun;
};

} // NUMERIC

#endif // NUMERIC_CHEBOPERATOR_H
