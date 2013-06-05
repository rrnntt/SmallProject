#ifndef NUMERIC_SCHRODINGER1D_H
#define NUMERIC_SCHRODINGER1D_H

#include "Numeric/DllExport.h"
#include "API/Algorithm.h"

namespace Numeric
{

//=================================================
// Forward declarations
//=================================================
class ChebOperator;
class ChebfunVector;
class GSLVector;
class GSLMatrix;

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
class NUMERIC_EXPORT Schrodinger1D: public API::Algorithm
{
public:
  Schrodinger1D();
  virtual std::string name() const {return "Schrodinger1D";}
protected:
  virtual void exec();
  void improve(ChebOperator *hamiltonian, ChebfunVector *basis, GSLVector &eigv, ChebfunVector **eigf)const;
  void getSortedIndex(const GSLVector &v, std::vector<size_t> &indx) const;
  void makeQuadrature(ChebfunVector *basis);
};

} // NUMERIC

#endif // NUMERIC_SCHRODINGER1D_H
