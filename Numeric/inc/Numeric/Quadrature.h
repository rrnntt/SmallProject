#ifndef NUMERIC_QUADRATURE_H
#define NUMERIC_QUADRATURE_H

#include "Numeric/DllExport.h"
#include "API/TableWorkspace.h"

namespace Numeric
{

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
  std::vector<double>* m_r; ///< the integration points
  std::vector<double>* m_w; ///< the integration weights
};

} // Numeric

#endif // NUMERIC_QUADRATURE_H
