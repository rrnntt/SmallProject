#ifndef NUMERIC_NONPOLYNOMIAL_H
#define NUMERIC_NONPOLYNOMIAL_H

#include "Numeric/DllExport.h"
#include "Numeric/Polynomial.h"

namespace Numeric
{

/**
  @brief This is a "generalized polynomial" in which the role of \f$x\f$ is played by some function of \f$x\f$.


  */
class NUMERIC_EXPORT NonPolynomial: public Polynomial
{
public:
    /// Default constructor
    NonPolynomial();
    /// Constructor
    NonPolynomial(int n, IFunction_sptr xfun);
    /// Returns the function's name
    virtual std::string name()const {return "NonPolynomial";}
    /// Returns the integral of the weight function
    virtual double weightIntegral() const;

    /* Own public methods */
    /// Set the \f$x\f$ function
    void setXFunction(IFunction_sptr xfun);
protected:
    /// Recalculate (re-fill) m_a, m_b, m_c
    virtual void updateABC() const;
    /// Return cost shared pointer to the weight function (creates weight function)
    virtual IFunction_const_sptr createWeightFunction() const;
    virtual IFunction_const_sptr createWeightDerivative() const;

private:

    IFunction_sptr m_xfun;
    double m_startX;
    double m_endX;

};

} // Numeric

#endif // NUMERIC_NONPOLYNOMIAL_H
