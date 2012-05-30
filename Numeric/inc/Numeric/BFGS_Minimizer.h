#ifndef NUMERIC_BFGS_MINIMIZERMINIMIZER_H_
#define NUMERIC_BFGS_MINIMIZERMINIMIZER_H_

//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Numeric/DllExport.h"
#include "Numeric/DerivMinimizer.h"

namespace Numeric
{
/** Implementing Broyden-Fletcher-Goldfarb-Shanno (BFGS) algorithm
    by wrapping the IFuncMinimizer interface around the GSL implementation of this algorithm.

    @author Anders Markvardsen, ISIS, RAL
    @date 13/1/2010

    Copyright &copy; 2009 ISIS Rutherford Appleton Laboratory & NScD Oak Ridge National Laboratory

    This file is part of Mantid.

    Mantid is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    Mantid is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    File change history is stored at: <https://svn.mantidproject.org/mantid/trunk/Code/Mantid>.
    Code Documentation is available at: <http://doxygen.mantidproject.org>
*/
class NUMERIC_EXPORT BFGS_Minimizer : public DerivMinimizer
{
public:
  /// Constructor.
  BFGS_Minimizer():DerivMinimizer()  {}
  /// Name of the minimizer.
  std::string name() const {return "BFGS_Minimizer";}

protected:

  /// Return a concrete type to initialize m_gslSolver with
  virtual const gsl_multimin_fdfminimizer_type* getGSLMinimizerType();

};


} // Numeric

#endif /*NUMERIC_BFGS_MINIMIZERMINIMIZER_H_*/
