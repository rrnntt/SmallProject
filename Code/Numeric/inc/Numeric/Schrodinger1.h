#ifndef NUMERIC_SCHRODINGER1_H
#define NUMERIC_SCHRODINGER1_H

#include "Numeric/DllExport.h"
#include "API/Algorithm.h"

namespace Numeric
{

/**
    Solves 1d Schrodinger equation with the operator
    
    -\beta*diff2 + V

    where \beta is a constant, V is a IFunction, diff2 is the second derivative operator.
    The solutions are found on an interval [StartX,EndX]

    Properties:
      input:
        - Beta - the \beta double parameter
        - VPot - the V function (IFunction)
        - StartX - the start of the interval
        - EndX - the end of the interval
      output:
        - Eigenvalues - TableWorkspace with eigenvalues 
        - Eigenvectors - ChebfunVector with eigenvectors
 */
class NUMERIC_EXPORT Schrodinger1: public API::Algorithm
{
public:
  Schrodinger1();
  virtual std::string name() const {return "Schrodinger1";}
protected:
  virtual void exec();
};

} // NUMERIC

#endif // NUMERIC_SCHRODINGER1_H
