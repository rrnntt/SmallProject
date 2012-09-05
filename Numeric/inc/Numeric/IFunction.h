#ifndef NUMERIC_IFUNCTION_H_
#define NUMERIC_IFUNCTION_H_

//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Numeric/DllExport.h"
#include "Numeric/FunctionDomain.h"
#include "Numeric/FunctionValues.h"
#include "Numeric/Jacobian.h"
#include "Kernel/PropertyClass.h"

#include <boost/shared_ptr.hpp>
#include <boost/variant.hpp>
#include <string>
#include <vector>

#ifdef _WIN32
  #pragma warning( disable: 4250 )
#endif

namespace Numeric
{
//----------------------------------------------------------------------
// Forward declaration
//----------------------------------------------------------------------
class ParameterTie;
class IConstraint;
class ParameterReference;

/** This is an interface to a fitting function - a semi-abstarct class.
    Functions derived from IFunction can be used with the Fit algorithm.
    IFunction defines the structure of a fitting funtion.

    A function has a number of named parameters (not arguments), type double, on which it depends.
    Parameters must be declared either in the constructor or in the init() method
    of a derived class with method declareParameter(...). Method nParams() returns 
    the number of declared parameters. A parameter can be accessed either by its name
    or the index. For example in case of Gaussian the parameters can be "Height",
    "PeakCentre" and "Sigma".

    To fit a function to a set of data its parameters must be adjusted so that the difference
    between the data and the corresponding function values were minimized. This is the aim
    of the Fit algorithm. But Fit does not work with the declared parameters directly.
    Instead it uses other - active - parameters. The active parameters can be a subset of the
    declared parameters or completely different ones. The rationale for this is following.
    The fitting parameters sometimes need to be fixed during the fit or "tied" (expressed
    in terms of other parameters). In this case the active parameters will be those
    declared parameters which are not tied in any sence. Also some of the declared parameters
    can be unsuitable for the use in a fitting algorithm. In this case different active parameters
    can be used in place of the inefficient declared parameters. An example is Gaussian where
    "Sigma" makes the fit unstable. So in the fit it can be replaced with variable Weight = 1 / Sigma
    which is more efficient. The number of active parameters (returned by nActive()) cannot be
    greater than nParams(). The function which connects the active parameters with the declared ones
    must be monotonic so that the forward and backward transformations between the two sets are
    single-valued (this is my understanding). At the moment only simple one to one transformations
    of Weight - Sigma type are allowed. More complecated cases of simultaneous transformations of
    several parameters are not supported.

    The active parameters can be accessed by their index. The implementations of the access method
    for both active and declared parameters must ensure that any changes to one of them 
    immediately reflected on the other so that the two are consistent at any moment.

    IFunction declares method nameOfActive(int i) which returns the name of the declared parameter
    corresponding to the i-th active parameter. I am not completely sure in the usefulness of it.

    IFunction provides methods for tying and untying parameters. Only the declared parameters can be 
    tied. A tied parameter cannot be active. When a parameter is tied it becomes inactive.
    This implies that the number of active parameters is variable and can change at runtime.

    Method addConstraint adds constraints on possible values of a declared parameter. Constraints
    and ties are used only in fitting.

    The main method of IFunction is called function(out,xValues,nData). It calculates nData output values
    out[i] at arguments xValues[i]. Implement functionDeriv method for the function to be used with
    fitting algorithms using derivatives. functionDeriv calculates patrial derivatives of the
    function with respect to the fitting parameters.

    Any non-fitting parameters can be implemented as attributes (class IFunction::Attribute). 
    An attribute can have one of three types: std::string, int, or double. The type is set at construction
    and cannot be changed later. To read or write the attributes there are two ways. If the type
    is known the type specific accessors can be used, e.g. asString(), asInt(). Otherwise the
    IFunction::AttributeVisitor can be used. It provides alternative virtual methods to access 
    attributes of each type. When creating a function from a string (using FunctionFactory::creaeInitialized(...))
    the attributes must be set first, before any fitting parameter, as the number and names of the parameters
    can depend on the attributes.
*/
class NUMERIC_EXPORT IFunction: public Kernel::PropertyClass
{
public:

  /**
   * Atribute visitor class. It provides a separate access method
   * for each attribute type. When applied to a particular attribue
   * the appropriate method will be used. The child classes must
   * implement the virtual AttributeVisitor::apply methods. See 
   * implementation of Attribute::value() method for an example.
   */
  template<typename T = void>
  class NUMERIC_EXPORT AttributeVisitor: public boost::static_visitor<T>
  {
  public:
    /// Virtual destructor
    virtual ~AttributeVisitor() {}
    /// implements static_visitor's operator() for std::string
    T operator()(std::string& str)const{return apply(str);}
    /// implements static_visitor's operator() for double
    T operator()(double& d)const{return apply(d);}
    /// implements static_visitor's operator() for int
    T operator()(int& i)const{return apply(i);}
  protected:
    /// Implement this mathod to access attribute as string
    virtual T apply(std::string&)const = 0;
    /// Implement this mathod to access attribute as double
    virtual T apply(double&)const = 0;
    /// Implement this mathod to access attribute as int
    virtual T apply(int&)const = 0;
  };

  /**
   * Const version of AttributeVisitor. 
   */
  template<typename T = void>
  class NUMERIC_EXPORT ConstAttributeVisitor: public boost::static_visitor<T>
  {
  public:
    /// Virtual destructor
    virtual ~ConstAttributeVisitor() {}
    /// implements static_visitor's operator() for std::string
    T operator()(std::string& str)const{return apply(str);}
    /// implements static_visitor's operator() for double
    T operator()(double& d)const{return apply(d);}
    /// implements static_visitor's operator() for int
    T operator()(int& i)const{return apply(i);}
  protected:
    /// Implement this mathod to access attribute as string
    virtual T apply(const std::string& str)const = 0;
    /// Implement this mathod to access attribute as double
    virtual T apply(const double& d)const = 0;
    /// Implement this mathod to access attribute as int
    virtual T apply(const int& i)const = 0;
  };

  /// Attribute is a non-fitting parameter.
  /// It can be one of the types: std::string, int, or double
  /// Examples: file name, polinomial order
  class NUMERIC_EXPORT Attribute
  {
  public:
    /// Create empty string attribute
    explicit Attribute():m_data(""), m_quoteValue(false) {}
    /// Create string attribute
    explicit Attribute(const std::string& str, bool quoteValue=false):m_data(str), m_quoteValue(quoteValue) {}
    /// Create int attribute
    explicit Attribute(const int& i):m_data(i){}
    /// Create double attribute
    explicit Attribute(const double& d):m_data(d){}
    /// Apply an attribute visitor
    template<typename T>
    T apply(AttributeVisitor<T>& v){return boost::apply_visitor(v,m_data);}
    /// Apply a const attribute visitor
    template<typename T>
    T apply(ConstAttributeVisitor<T>& v)const{return boost::apply_visitor(v,m_data);}
    /// Returns type of the attribute
    std::string type()const;
    /// Returns the attribute value as a string
    std::string value()const;
    /// Returns string value if attribute is a string, throws exception otherwise
    std::string asString()const;
    /// Returns a string value that is guarenteed to be quoted for use in places where the string is used as the displayed value.
    std::string asQuotedString() const;
    /// Returns a string value that is guarenteed to be unquoted.
    std::string asUnquotedString() const;
    /// Returns int value if attribute is a int, throws exception otherwise
    int asInt()const;
    /// Returns double value if attribute is a double, throws exception otherwise
    double asDouble()const;
    /// Sets new value if attribute is a string
    void setString(const std::string& str);
    /// Sets new value if attribute is a double
    void setDouble(const double&);
    /// Sets new value if attribute is a int
    void setInt(const int&);
    /// Set value from a string.
    void fromString(const std::string& str);
  private:
    /// The data holder as boost variant
    mutable boost::variant<std::string,int,double> m_data;
    /// Flag indicating if the string value should be returned quoted
    bool m_quoteValue;
  };

  //---------------------------------------------------------//

  /// Constructor
  IFunction(){}
  /// Virtual destructor
  virtual ~IFunction();

  /// Implement PropertyClass virtual method
  std::string asString()const {return IFunction::asString(false);}

  /// Returns the function's name
  virtual std::string name()const = 0;
  /// Writes itself into a string
  virtual std::string asString(bool fmt, size_t level = 0)const;

  /// The categories the Fit function belong to.
  /// Categories must be listed as a semi colon separated list.
  /// For example: "General, Muon\\Custom" which adds 
  /// a function to the category "General" and the sub-category
  /// "Muon\\Custom" 
  virtual const std::string category() const { return "General";}
  /// Function to return all of the categories that contain this algorithm
  virtual const std::vector<std::string> categories() const;
  /// Function to return the sperator token for the category string. A default implementation ';' is provided
  virtual const std::string categorySeparator() const {return ";";}

  /// Function you want to fit to. 
  /// @param domain :: The buffer for writing the calculated values. Must be big enough to accept dataSize() values
  virtual void function(const FunctionDomain& domain, FunctionValues& values)const = 0;
  /// Derivatives of function with respect to active parameters
  virtual void functionDeriv(const FunctionDomain& domain, Jacobian& jacobian);

  /** @name Function parameters */
  //@{
  /// Set i-th parameter
  virtual void setParameter(size_t, const double& value, bool explicitlySet = true) = 0;
  /// Set i-th parameter description
  virtual void setParameterDescription(size_t, const std::string& description) = 0;
  /// Get i-th parameter
  virtual double getParameter(size_t i)const = 0;
  /// Set parameter by name.
  virtual void setParameter(const std::string& name, const double& value, bool explicitlySet = true) = 0;
  /// Set description of parameter by name.
  virtual void setParameterDescription(const std::string& name, const std::string& description) = 0;
  /// Get parameter by name.
  virtual double getParameter(const std::string& name)const = 0;
  /// Total number of parameters
  virtual size_t nParams()const = 0;
  /// Returns the index of parameter name
  virtual size_t parameterIndex(const std::string& name)const = 0;
  /// Returns the name of parameter i
  virtual std::string parameterName(size_t i)const = 0;
  /// Returns the description of parameter i
  virtual std::string parameterDescription(size_t i)const = 0;
  /// Checks if a parameter has been set explicitly
  virtual bool isExplicitlySet(size_t i)const = 0;
  /// Get the fitting error for a parameter
  virtual double getError(size_t i) const = 0;
  /// Set the fitting error for a parameter
  virtual void setError(size_t i, double err) = 0;

  /// Check if a declared parameter i is fixed
  virtual bool isFixed(size_t i)const = 0;
  /// Removes a declared parameter i from the list of active
  virtual void fix(size_t i) = 0;
  /// Restores a declared parameter i to the active status
  virtual void unfix(size_t i) = 0;

  /// Return parameter index from a parameter reference. Usefull for constraints and ties in composite functions
  virtual size_t getParameterIndex(const ParameterReference& ref)const = 0;
  /// Return a vector with all parameter names
  std::vector<std::string> getParameterNames() const ;
  //@}

  /** @name Active parameters */
  //@{
  /// Value of i-th active parameter. Override this method to make fitted parameters different from the declared
  virtual double activeParameter(size_t i)const;
  /// Set new value of i-th active parameter. Override this method to make fitted parameters different from the declared
  virtual void setActiveParameter(size_t i, double value);
  /// Returns the name of active parameter i
  virtual std::string nameOfActive(size_t i)const;
  /// Returns the name of active parameter i
  virtual std::string descriptionOfActive(size_t i)const;
  /// Check if an active parameter i is actually active
  virtual bool isActive(size_t i)const {return !isFixed(i);}
  //@}


  /// Tie a parameter to other parameters (or a constant)
  virtual ParameterTie* tie(const std::string& parName, const std::string& expr);
  /// Add several ties
  virtual void addTies(const std::string& ties);
  /// Apply the ties
  virtual void applyTies() = 0;
  /// Removes the tie off a parameter
  virtual void removeTie(const std::string& parName);
  /// Remove all ties
  virtual void clearTies() = 0;
  /// Removes i-th parameter's tie
  virtual bool removeTie(size_t i) = 0;
  /// Get the tie of i-th parameter
  virtual ParameterTie* getTie(size_t i)const = 0;


  /// Add a list of conatraints from a string
  virtual void addConstraints(const std::string& str);
  /// Add a constraint to function
  virtual void addConstraint(IConstraint* ic) = 0;
  /// Get constraint of i-th parameter
  virtual IConstraint* getConstraint(size_t i)const = 0;
  /// Remove a constraint
  virtual void removeConstraint(const std::string& parName) = 0;

  /// Set the parameters of the function to satisfy the constraints of
  /// of the function. For example
  /// for a BoundaryConstraint this if param value less than lower boundary
  /// it is set to that value and vice versa for if the param value is larger
  /// than the upper boundary value.
  virtual void setParametersToSatisfyConstraints() {};

  /// Returns the number of attributes associated with the function
  virtual size_t nAttributes()const{return 0;}
  /// Returns a list of attribute names
  virtual std::vector<std::string> getAttributeNames()const{return std::vector<std::string>();}
  /// Return a value of attribute attName
  virtual Attribute getAttribute(const std::string& attName)const
  {
    throw std::invalid_argument("Attribute "+attName+" not found in function "+this->name());
  }
  /// Set a value to attribute attName
  virtual void setAttribute(const std::string& attName,const Attribute& )
  {
    throw std::invalid_argument("Attribute "+attName+" not found in function "+this->name());
  }
  /// Check if attribute attName exists
  virtual bool hasAttribute(const std::string& attName)const { (void)attName; return false;}
  template<typename T>
  void setAttributeValue(const std::string& attName,const T& value){setAttribute(attName,Attribute(value));}

  /// Calculate numerical derivatives
  void calNumericalDeriv(const FunctionDomain& domain, Jacobian& out);

protected:

  /// Function initialization. Declare function parameters in this method.
  virtual void init(){};
  /// Declare a new parameter
  virtual void declareParameter(const std::string& name, double initValue = 0, const std::string& description="") = 0;

  /// Create an instance of a tie without actually tying it to anything
  //virtual ParameterTie* createTie(const std::string& parName);
  /// Add a new tie
  virtual void addTie(ParameterTie* tie) = 0;

  friend class ParameterTie;
  friend class CompositeFunction;

  /// Values storage for numeric derivatives
  FunctionValues m_minusStep;
  /// Values storage for numeric derivatives
  FunctionValues m_plusStep;

};

///shared pointer to the function base class
typedef boost::shared_ptr<IFunction> IFunction_sptr;
///shared pointer to the function base class (const version)
typedef boost::shared_ptr<const IFunction> IFunction_const_sptr;

} // namespace Numeric

#endif /*NUMERIC_IFUNCTION_H_*/
