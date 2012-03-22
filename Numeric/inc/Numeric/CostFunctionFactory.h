#ifndef NUMERIC_COSTFUNCTIONFACTORY_H_
#define NUMERIC_COSTFUNCTIONFACTORY_H_

//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include <vector>
#include "Numeric/DllExport.h"
#include "Kernel/DynamicFactory.h"
#include "API/Singleton.h"

namespace Kernel
{
  class Logger;
}

namespace Numeric
{

//----------------------------------------------------------------------
// More forward declarations
//----------------------------------------------------------------------
  class ICostFunction;

/** @class CostFunctionFactoryImpl

    The CostFunctionFactory class is in charge of the creation of concrete
    instances of minimizers. It inherits most of its implementation from
    the Dynamic Factory base class.
    It is implemented as a singleton class.
    
*/

  class NUMERIC_EXPORT CostFunctionFactory : public Kernel::DynamicFactory<ICostFunction>, protected API::Singleton
  {
  public:
    /// Constructor
    CostFunctionFactory();
    /**Creates an instance of a function
     * @param type :: The function's type
     * @return A pointer to the created function
     */
    ICostFunction* createFunction(const std::string& type) const;


  private:

    ///static reference to the logger class
    Kernel::Logger& g_log;
  };
  
        
} // namespace Numeric

#endif /*NUMERIC_COSTFUNCTIONFACTORY_H_*/
