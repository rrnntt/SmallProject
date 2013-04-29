#ifndef NUMERIC_QUADRATURE_H
#define NUMERIC_QUADRATURE_H

#include "Numeric/DllExport.h"
#include "Numeric/IFunction.h"
#include "Numeric/GSLVector.h"

#include "API/TableWorkspace.h"

namespace Numeric
{

class GSLMatrix;

/** 
 * Table workspace containing all necessary data for an n-point quadrature
 * calculations. The number of rows == n.
 * 
 * The workspace must have columns:
 * - double "r" with the points
 * - double "w" with the weights
 * 
 * After creating all the columns init() must be called.
 */
class NUMERIC_EXPORT Quadrature: public API::TableWorkspace
{
public:
  /// Type for function values at integration points
  typedef std::vector< std::vector<double>* > FuncVector;
  Quadrature();
  ~Quadrature();
  virtual std::string id()const {return "Quadrature";}
  void init();
  /// Number of the integration points
  size_t size() const {return rowCount();}
  /// Calculate kinetic energy matrix elemetnt
  double calcKinet(size_t i, size_t j, const double& beta) const;
  double calcPot(size_t i, size_t j, const std::vector<double>& vpot) const;
  /// Build a hamiltonian matrix
  void buildHamiltonian(const double& beta, const std::vector<double>& vpot, GSLMatrix& H) const;
  void buildHamiltonian(const double& beta, const IFunction& vpot, GSLMatrix& H) const;
protected:
  // pointers to data in the table columns. init() sets these pointers up.
  std::vector<double>* m_r; ///< the integration points
  std::vector<double>* m_w; ///< the integration weights
  FuncVector m_funs;        ///< basis function values at integration points, m_funs.size()==n
  FuncVector m_derivs;      ///< derivatives of basis function values at integration points
};

} // Numeric

#endif // NUMERIC_QUADRATURE_H
