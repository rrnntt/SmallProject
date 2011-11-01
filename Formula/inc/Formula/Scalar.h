#ifndef SCALAR_H
#define SCALAR_H

#include "Formula/DllExport.h"
#include "Formula/Variable.h"

namespace Formula
{

class FORMULA_EXPORT Scalar: public Variable
{
public:
  Scalar();
  Scalar(const double& d);
  Scalar(const Scalar& sc);
  Scalar(double* d);
  ~Scalar();
  std::string type()const{return "Scalar";}
  std::string toString()const;
  Variable* clone()const;
  bool isLiteral(const std::string& nm)const;
  bool setValue(const std::string& val);
  bool setValue(const Variable& o);
  bool setValue(const double* d);
  bool setValue(const double& d);
  void setRandomValue();
  bool operator==(const Variable&);
  bool compare(const Variable&,double absTol = -1., double relTol = -1.);
  void setExpression(boost::shared_ptr<Expression> expr);

  operator double()const{return m_value;}

  /* Scalar methods*/
  void negate();
  void negate(const Scalar& s);
  void plus(const Scalar& s);
  void minus(const Scalar& s);
  void times(const Scalar& s);
  void divide(const Scalar& s);
  void power(const Scalar& s);
  void setZero(){m_value = 0.;}
  void setOne(){m_value = 1.;}
private:
  double m_store_value;
  double& m_value;
};

typedef boost::shared_ptr<Scalar> Scalar_ptr;
typedef boost::shared_ptr<const Scalar> Scalar_cptr;

} // Formula

#endif // SCALAR_H
