#ifndef NUMERIC_CONSTRAINTFACTORY_H_
#define NUMERIC_CONSTRAINTFACTORY_H_

//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include <vector>
#include "Numeric/DllExport.h"
#include "Kernel/DynamicFactory.h"
#include "API/Singleton.h"

namespace Kernel
{
  class EParser;
}

namespace Numeric
{

//----------------------------------------------------------------------
// More forward declarations
//----------------------------------------------------------------------
  class IConstraint;
  class IFunction;


  class NUMERIC_EXPORT ConstraintFactory : public Kernel::DynamicFactory<IConstraint>, protected API::Singleton
  {
  public:
    virtual std::string name() const {return "ConstraintFactory";}
    static ConstraintFactory& instance();
    /**Creates an instance of a Constraint
     * @param fun :: The function
     * @param input :: The creation expression
     * @return A pointer to the created Constraint
     */
    IConstraint* createInitialized(IFunction* fun, const std::string& input) const;
    /**Creates an instance of a Constraint
     * @param fun :: The function
     * @param expr :: The creation expression
     * @return A pointer to the created Constraint
     */
    IConstraint* createInitialized(IFunction* fun, const Kernel::EParser& expr) const;

  private:

    /// Private Constructor for singleton class
    ConstraintFactory();
    /// Private copy constructor - NO COPY ALLOWED
    ConstraintFactory(const ConstraintFactory&);
    /// Private assignment operator - NO ASSIGNMENT ALLOWED
    ConstraintFactory& operator = (const ConstraintFactory&);

  };

} // Numeric  

#endif /*MANTID_API_CONSTRAINTFACTORY_H_*/
