#ifndef NUMERIC_QUADRATURE_H
#define NUMERIC_QUADRATURE_H

#include "Numeric/DllExport.h"
#include "API/TableWorkspace.h"

namespace Numeric
{

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
  Quadrature();
  ~Quadrature();
  virtual std::string id()const {return "Quadrature";}
  void init();
  /// Calculate kinetic energy matrix elemetnt
  double calcKinet(size_t i, size_t j, const double& beta) const;
  double calcPot(size_t i, size_t j, const std::vector<double>& vpot) const;
protected:
  typedef std::vector< std::vector<double>* > FuncVector;
  std::vector<double>* m_r; ///< the integration points
  std::vector<double>* m_w; ///< the integration weights
  FuncVector m_funs;        ///< basis function values at integration points, m_funs.size()==n
};

} // Numeric

#endif // NUMERIC_QUADRATURE_H
