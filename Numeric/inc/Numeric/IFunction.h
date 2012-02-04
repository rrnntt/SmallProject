#ifndef NUMERIC_IFUNCTION_H
#define NUMERIC_IFUNCTION_H

#include "Numeric/DllExport.h"
#include "Numeric/FunctionDomain.h"
#include "Numeric/Jacobian.h"

#include <boost/variant.hpp>
#include <string>
#include <vector>
#include <stdexcept>

namespace Numeric
{

class NUMERIC_EXPORT IFunction
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
  virtual ~IFunction(){}

  /// Returns the function's name
  virtual std::string name()const = 0;
  /// Writes itself into a string
  virtual std::string asString()const;
  /// The string operator
  virtual operator std::string()const{return asString();}

  /// Function you want to fit to. 
  /// @param domain :: The buffer for writing the calculated values. Must be big enough to accept dataSize() values
  virtual void function(FunctionDomain& domain)const = 0;
  /// Derivatives of function with respect to active parameters
  virtual void functionDeriv(FunctionDomain& domain, Jacobian& jacobian);

  /// Get i-th parameter
  virtual double getParameter(size_t i)const = 0;
  /// Get parameter by name.
  virtual double getParameter(const std::string& name)const = 0;
  /// Set i-th parameter
  virtual void setParameter(size_t, const double& value) = 0;
  /// Set parameter by name.
  virtual void setParameter(const std::string& name, const double& value) = 0;
  /// Total number of parameters
  virtual size_t nParams()const = 0;
  /// Returns the index of parameter name
  virtual size_t parameterIndex(const std::string& name)const = 0;
  /// Returns the name of parameter i
  virtual std::string parameterName(size_t i)const = 0;

  /// Number of active (in terms of fitting) parameters
  virtual size_t nActive()const = 0;
  /// Value of i-th active parameter. Override this method to make fitted parameters different from the declared
  virtual double activeParameter(size_t i)const = 0;
  /// Set new value of i-th active parameter. Override this method to make fitted parameters different from the declared
  virtual void setActiveParameter(size_t i, double value) = 0;
  /// Update parameters after a fitting iteration
  //virtual void updateActive(const double* in);
  /// Returns the name of active parameter i
  virtual std::string nameOfActive(size_t i)const = 0;

  /// Check if declared parameter i is fixed
  virtual bool isFixed(size_t i)const = 0;
  /// Fixes declared parameter i
  virtual void fix(size_t i) = 0;
  /// Restores declared parameter i for fitting
  virtual void unfix(size_t i) = 0;

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

protected:

  /// Declare a new parameter
  virtual void declareParameter(const std::string& name, double initValue = 0) = 0;
  void validateParameterIndex(size_t i) const;
  void validateActiveParameterIndex(size_t i) const;

};

typedef boost::shared_ptr<IFunction> IFunction_ptr;

} // namespace Numeric
#endif // NUMERIC_IFUNCTION_H
