#ifndef NUMERIC_MAKEQUADRATURE_H
#define NUMERIC_MAKEQUADRATURE_H

#include "Numeric/DllExport.h"
#include "API/Algorithm.h"

namespace Numeric
{

class Polynomial;

class NUMERIC_EXPORT MakeQuadrature: public API::Algorithm
{
public:
  MakeQuadrature();
  virtual std::string name() const {return "MakeQuadrature";}
protected:
  virtual void exec();
  virtual void applyProperty(const std::string& name);
  void makeChebyshev();
  void makeHermite();
  void makeLaguerre();
  void makeLegendre();
  void makeCustom();
  void makeQuadrature(const Polynomial& P);
};

} // NUMERIC

#endif // NUMERIC_MAKEQUADRATURE_H
