#ifndef NUMERIC_FUNCTIONFACTORY_H_
#define NUMERIC_FUNCTIONFACTORY_H_

//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include <vector>
#include "Numeric/DllExport.h"
#include "Kernel/DynamicFactory.h"
#include "Kernel/Logger.h"
#include "API/Singleton.h"

#include <boost/shared_ptr.hpp>

namespace Kernel
{
  class EParser;
}

namespace Numeric
{

//----------------------------------------------------------------------
// More forward declarations
//----------------------------------------------------------------------
  class IFunction;
  class CompositeFunction;

/** @class FunctionFactoryImpl

    The FunctionFactory class is in charge of the creation of concrete
    instances of fitting functions. It inherits most of its implementation from
    the Dynamic Factory base class.
    It is implemented as a singleton class.
*/

  class NUMERIC_EXPORT FunctionFactory : public Kernel::DynamicFactory<IFunction>, protected API::Singleton
  {
  public:
    virtual std::string name() const {return "Numeric::FunctionFactory";}
    static FunctionFactory& instance();

    ///Creates an instance of a function
    boost::shared_ptr<IFunction> createFitFunction(const std::string& input) const;

    /// Query available functions based on the template type
    template<typename FunctionType>
    std::vector<std::string> getFunctionNames() const;

  private:

    /// Private Constructor for singleton class
    FunctionFactory();
    /// Private copy constructor - NO COPY ALLOWED
    FunctionFactory(const FunctionFactory&);
    /// Private assignment operator - NO ASSIGNMENT ALLOWED
    FunctionFactory& operator = (const FunctionFactory&);

    /// These methods shouldn't be used to create functions
    using Kernel::DynamicFactory<IFunction>::create;

    /// Create a simple function
    boost::shared_ptr<IFunction> createSimple(
      const Kernel::EParser& expr, 
      std::map<std::string,std::string>& parentAttributes
      )const;
    /// Create a composite function
    boost::shared_ptr<CompositeFunction> createComposite(
      const Kernel::EParser& expr, 
      std::map<std::string,std::string>& parentAttributes
      )const;
    ///Creates an instance of a function
    boost::shared_ptr<IFunction> createFitFunction(const Kernel::EParser& expr) const;

    /// Throw an exception
    void inputError(const std::string& str="")const;
    /// Add constraints to the created function
    void addConstraints(boost::shared_ptr<IFunction> fun,const Kernel::EParser& expr)const;
    /// Add a single constraint to the created function
    void addConstraint(boost::shared_ptr<IFunction> fun,const Kernel::EParser& expr)const;
    /// Add ties to the created function
    void addTies(boost::shared_ptr<IFunction> fun,const Kernel::EParser& expr)const;
    /// Add a tie to the created function
    void addTie(boost::shared_ptr<IFunction> fun,const Kernel::EParser& expr)const;

    ///static reference to the logger class
    Kernel::Logger& g_log;

  };

  /**
   * Query available functions based on the template type
   * @tparam FunctionType :: The type of the functions to list 
   * @returns A vector of the names of the functions matching the template type 
   */
  //template<typename FunctionType>
  //std::vector<std::string> FunctionFactory::getFunctionNames() const
  //{
  //  const std::vector<std::string> names = this->getKeys();
  //  std::vector<std::string> typeNames;
  //  typeNames.reserve(names.size());
  //  for( std::vector<std::string>::const_iterator it = names.begin(); 
  //       it != names.end(); ++it )
  //  {
  //    IFunction_sptr func = this->createFitFunction(*it);
  //    if ( func && dynamic_cast<FunctionType*>(func.get()) )
  //    {
  //      typeNames.push_back(*it);
  //    }
  //  }
  //  return typeNames;
  //}
        
} // namespace Numeric

/**
 * Macro for declaring a new type of function to be used with the FunctionFactory
 */
#define DECLARE_FUNCTION(classname) \
        namespace { \
        int register_function_##classname( \
  ((Numeric::FunctionFactory::instance().subscribe<classname>(#classname)) \
        , 0)); \
        }

#endif /*MANTID_API_FUNCTIONFACTORY_H_*/
