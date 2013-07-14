//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Numeric/IFunction.h"
#include "Numeric/Jacobian.h"
#include "Numeric/IConstraint.h"
#include "Numeric/ParameterTie.h"
#include "Numeric/ConstraintFactory.h"

#include <boost/lexical_cast.hpp>

#include <limits>
#include <sstream>
#include <iostream> 

namespace Numeric
{
  
/**
 * Destructor
 */
IFunction::~IFunction()
{
}

/** Base class implementation of derivative IFunction throws error. This is to check if such a function is provided
    by derivative class. In the derived classes this method must return the derivatives of the function
    with respect to the fit parameters. If this method is not reimplemented the derivative free simplex minimization
    algorithm is used or the derivatives are computed numerically.
 * @param domain :: The domain of the function
 * @param jacobian :: Pointer to a Jacobian matrix. If it is NULL the method is called in order to check whether it's implemented or not.
 *      If the derivatives are implemented the method must simply return, otherwise it must throw Kernel::Exception::NotImplementedError.
 */
void IFunction::functionDeriv(const FunctionDomain& domain, Jacobian& jacobian)
{
  calNumericalDeriv(domain, jacobian);
}

/**
 * Ties a parameter to other parameters
 * @param parName :: The name of the parameter to tie.
 * @param expr ::    A math expression 
 * @return newly ties parameters
 */
ParameterTie* IFunction::tie(const std::string& parName,const std::string& expr)
{
  ParameterTie* ti = new ParameterTie(this,parName,expr);
  addTie(ti);
  this->fix(getParameterIndex(*ti));
  return ti;
}

/**
 * Add ties to the function.
 * @param ties :: Comma-separated list of name=value pairs where name is a parameter name and value
 *  is a math expression tying the parameter to other parameters or a constant.
 */
void IFunction::addTies(const std::string& ties)
{
  //Expression list;
  //list.parse(ties);
  //list.toList();
  //for(auto t = list.begin(); t != list.end(); ++t)
  //{
  //  if (t->name() == "=" && t->size() >= 2)
  //  {
  //    size_t n = t->size() - 1;
  //    const std::string value = (*t)[n].str();
  //    for( size_t i = n; i != 0; )
  //    {
  //      --i;
  //      this->tie( (*t)[i].name(), value );
  //    }
  //  }
  //}
}

/** Removes the tie off a parameter. The parameter becomes active
 * This method can be used when constructing and editing the IFunction in a GUI
 * @param parName :: The name of the parameter which ties will be removed.
 */
void IFunction::removeTie(const std::string& parName)
{
  size_t i = parameterIndex(parName);
  this->removeTie(i);
}

/**
 * Writes a string that can be used in Fit.IFunction to create a copy of this IFunction
 * @param fmt :: Formatting flag. Set to true to ctreated formatted output.
 * @return string representation of the function
 */
std::string IFunction::asString(bool fmt, size_t level)const
{
  std::string nl = fmt? "\n" : "";
  std::string padding = fmt && level ? std::string(level*2,' ') : "";
  std::string padding2 = padding + "  ";

  std::ostringstream ostr;
  
  // write function name
  ostr << padding << this->name() << "(" << nl;
  bool needComma = false;

  // write function attributes
  std::vector<std::string> attr = this->getAttributeNames();
  for(size_t i=0;i<attr.size();i++)
  {
    std::string attName = attr[i];
    std::string attValue = this->getAttribute(attr[i]).value();
    if (!attValue.empty())
    {
      if (needComma)
      {
        ostr << ',' << nl;
      }
      needComma = true;
      ostr << padding2 << attName << '=' << attValue;
    }
  }

  // write function parameters
  for(size_t i=0;i<nParams();i++)
  {
    if (needComma)
    {
      ostr << ',' << nl;
    }
    needComma = true;
    ostr << padding2 << parameterName(i) << '=' << getParameter(i);
  }

  // write constraints
  std::string constraints;
  for(size_t i=0;i<nParams();i++)
  {
    const IConstraint* c = getConstraint(i);
    if (c)
    {
      std::string tmp = c->asString();
      if (!tmp.empty())
      {
        if (!constraints.empty())
        {
          constraints += ",";
        }
        constraints += tmp;
      }
    }
  }
  if (!constraints.empty())
  {
    ostr << ",constraints=(" << constraints << ")";
  }

  // write ties
  std::string ties;
  for(size_t i=0;i<nParams();i++)
  {
    const ParameterTie* tie = getTie(i);
    if (tie)
    {
      std::string tmp = tie->asString(this);
      if (!tmp.empty())
      {
        if (!ties.empty())
        {
          ties += ",";
        }
        ties += tmp;
      }
    }
  }
  if (!ties.empty())
  {
    ostr << ",ties=(" << ties << ")";
  }
  ostr << nl << padding << ")";
  return ostr.str();
}

/** Add a list of conatraints from a string
 * @param str :: A comma-separated list of name=expr pairs, where name is a parameter name and 
 *  expr is a constraint expression.
 */
void IFunction::addConstraints(const std::string& str)
{
  //Expression list;
  //list.parse(str);
  //list.toList();
  //for(auto expr = list.begin(); expr != list.end(); ++expr)
  //{
  //  IConstraint* c = ConstraintFactory::Instance().createInitialized(this,*expr);
  //  this->addConstraint(c);
    //}
}

size_t IFunction::nAttributes() const
{
    return m_attributes.size();
}

std::vector<std::string> IFunction::getAttributeNames() const
{
    std::vector<std::string> out;
    for(auto att = m_attributes.begin(); att != m_attributes.end(); ++att)
    {
        out.push_back(att->first);
    }
    return out;
}

/**
  Get an attribute if it exists. If it doen't then throw.
  */
IFunction::Attribute IFunction::getAttribute(const std::string &attName) const
{
    if ( !hasAttribute(attName) )
    {
        throw std::runtime_error("Attribute "+attName+" does not exist.");
    }
    return m_attributes[attName];
}

bool IFunction::hasAttribute(const std::string &attName) const
{
    return m_attributes.find(attName) != m_attributes.end();
}

/**
 * Return a vector with all parameter names.
 */
std::vector<std::string> IFunction::getParameterNames() const
{
  std::vector<std::string> out;
  for(size_t i = 0; i < nParams(); ++i)
  {
    out.push_back(parameterName(i));
  }
  return out;
}

/// Function to return all of the categories that contain this function
const std::vector<std::string> IFunction::categories() const
{
  std::vector < std::string > res;
  //Poco::StringTokenizer tokenizer(category(), categorySeparator(),
  //    Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
  //Poco::StringTokenizer::Iterator h = tokenizer.begin();

  //for (; h != tokenizer.end(); ++h)
  //{
  //  res.push_back(*h);
  //}

  return res;
}

/**
 * Operator <<
 * @param ostr :: The output stream
 * @param f :: The IFunction
 */
std::ostream& operator<<(std::ostream& ostr,const IFunction& f)
{
  ostr << f.asString();
  return ostr;
}

/**
 * Const attribute visitor returning the type of the attribute
 */
class AttType: public IFunction::ConstAttributeVisitor<std::string>
{
protected:
  /// Apply if string
  std::string apply(const std::string&)const{return "std::string";}
  /// Apply if int
  std::string apply(const int&)const{return "int";}
  /// Apply if double
  std::string apply(const double&)const{return "double";}
};

std::string IFunction::Attribute::type()const
{
  AttType tmp;
  return apply(tmp);
}

/**
 * Const attribute visitor returning the type of the attribute
 */
class AttValue: public IFunction::ConstAttributeVisitor<std::string>
{
public:
  AttValue(bool quoteString=false) : 
    IFunction::ConstAttributeVisitor<std::string>(),
    m_quoteString(quoteString) 
  {
  }

protected:
  /// Apply if string
  std::string apply(const std::string& str)const
  {
    return (m_quoteString) ? std::string("\"" + str + "\"") : str;
  }
  /// Apply if int
  std::string apply(const int& i)const{return boost::lexical_cast<std::string>(i);}
  /// Apply if double
  std::string apply(const double& d)const{return boost::lexical_cast<std::string>(d);}

private:
  /// Flag to quote a string value returned
  bool m_quoteString;
};

std::string IFunction::Attribute::value()const
{
  AttValue tmp(m_quoteValue);
  return apply(tmp);
}

std::string IFunction::Attribute::asString()const
{
  if( m_quoteValue ) return asQuotedString();
  
  try
  {
    return boost::get<std::string>(m_data);
  }
  catch(...)
  {
    throw std::runtime_error("Trying to access a "+type()+" attribute "
      "as string");
  }
}

std::string IFunction::Attribute::asQuotedString()const
{
  std::string attr;

  try
  {
    attr = boost::get<std::string>(m_data);
  }
  catch(...)
  {
    throw std::runtime_error("Trying to access a "+type()+" attribute "
      "as string");
  }
  std::string quoted(attr);
  if( *(attr.begin()) != '\"' ) quoted = "\"" + attr;
  if( *(quoted.end() - 1) != '\"' ) quoted += "\"";

  return quoted;
}

std::string IFunction::Attribute::asUnquotedString()const
{
  std::string attr;

  try
  {
    attr = boost::get<std::string>(m_data);
  }
  catch(...)
  {
    throw std::runtime_error("Trying to access a "+type()+" attribute "
      "as string");
  }
  std::string unquoted(attr);
  if( *(attr.begin()) == '\"' ) unquoted = std::string(attr.begin() + 1, attr.end() - 1);
  if( *(unquoted.end() - 1) == '\"' ) unquoted = std::string(unquoted.begin(), unquoted.end() - 1);
  
  return unquoted;
}

int IFunction::Attribute::asInt()const
{
  try
  {
    return boost::get<int>(m_data);
  }
  catch(...)
  {
    throw std::runtime_error("Trying to access a "+type()+" attribute "
      "as int");
  }
}

double IFunction::Attribute::asDouble()const
{
  try
  {
    return boost::get<double>(m_data);
  }
  catch(...)
  {
    throw std::runtime_error("Trying to access a "+type()+" attribute "
      "as double");
  }
}

/** Sets new value if attribute is a string. If the type is wrong 
 * throws an exception
 * @param str :: The new value
 */
void IFunction::Attribute::setString(const std::string& str)
{
  try
  {
    boost::get<std::string>(m_data) = str;
  }
  catch(...)
  {
    throw std::runtime_error("Trying to access a "+type()+" attribute "
      "as string");
  }
}

/** Sets new value if attribute is a double. If the type is wrong 
 * throws an exception
 * @param d :: The new value
 */
void IFunction::Attribute::setDouble(const double& d)
{
  try
  {
    boost::get<double>(m_data) = d;
  }
  catch(...)
  {
    throw std::runtime_error("Trying to access a "+type()+" attribute "
      "as double");
  }
}

/** Sets new value if attribute is an int. If the type is wrong 
 * throws an exception
 * @param i :: The new value
 */
void IFunction::Attribute::setInt(const int& i)
{
  try
  {
    boost::get<int>(m_data) = i;
  }
  catch(...)
  {
    throw std::runtime_error("Trying to access a "+type()+" attribute "
      "as int");
  }
}

/**
 * Attribute visitor setting new value to an attribute
 */
class SetValue: public IFunction::AttributeVisitor<>
{
public:
  /**
   * Constructor
   * @param value :: The value to set
   */
  SetValue(const std::string& value):m_value(value){}
protected:
  /// Apply if string
  void apply(std::string& str)const{str = m_value;}
  /// Apply if int
  void apply(int& i)const
  {
    std::istringstream istr(m_value+" ");
    istr >> i;
    if (!istr.good()) throw std::invalid_argument("Failed to set int attribute "
      "from string "+m_value);
  }
  /// Apply if double
  void apply(double& d)const
  {
    std::istringstream istr(m_value+" ");
    istr >> d;
    if (!istr.good()) throw std::invalid_argument("Failed to set double attribute "
      "from string "+m_value);
  }
private:
  std::string m_value; ///<the value as a string
};

/** Set value from a string. Throws exception if the string has wrong format
 * @param str :: String representation of the new value
 */
void IFunction::Attribute::fromString(const std::string& str)
{
  SetValue tmp(str);
  apply(tmp);
}

/// Value of i-th active parameter. Override this method to make fitted parameters different from the declared
double IFunction::activeParameter(size_t i)const 
{
  if ( !isActive(i) )
  {
    throw std::runtime_error("Attempt to use an inactive parameter");
  }
  return getParameter(i);
}

/// Set new value of i-th active parameter. Override this method to make fitted parameters different from the declared
void IFunction::setActiveParameter(size_t i, double value) 
{
  if ( !isActive(i) )
  {
    throw std::runtime_error("Attempt to use an inactive parameter");
  }
  setParameter(i,value);
}

/// Returns the name of active parameter i
std::string IFunction::nameOfActive(size_t i)const 
{
  if ( !isActive(i) )
  {
    throw std::runtime_error("Attempt to use an inactive parameter");
  }
  return parameterName(i);
}

/// Returns the name of active parameter i
std::string IFunction::descriptionOfActive(size_t i)const 
{
  if ( !isActive(i) )
  {
    throw std::runtime_error("Attempt to use an inactive parameter");
  }
  return parameterDescription(i);
}

/** Calculate numerical derivatives.
 * @param out :: Derivatives
 * @param xValues :: X values for data points
 * @param nData :: Number of data points
 */
void IFunction::calNumericalDeriv(const FunctionDomain& domain, Jacobian& jacobian)
{
    const double minDouble = std::numeric_limits<double>::min();
    const double epsilon = std::numeric_limits<double>::epsilon() * 100;
    double stepPercentage = 0.001; // step percentage
    double step; // real step
    double cutoff = 100.0*minDouble/stepPercentage;
    size_t nParam = nParams();
    size_t nData = domain.size();

    // allocate memory if not already done
    if (m_minusStep.size() != domain.size())
    {
      m_minusStep.reset(domain);
      m_plusStep.reset(domain);
    }

    applyTies(); // just in case
    function(domain,m_minusStep);

    for (size_t iP = 0; iP < nParam; iP++)
    {
      if ( isActive(iP) )
      {
        const double& val = activeParameter(iP);
        if (fabs(val) < cutoff)
        {
          step = epsilon;
        }
        else
        {
          step = val*stepPercentage;
        }

        double paramPstep = val + step;
        setActiveParameter(iP, paramPstep);
        applyTies(); 
        function(domain,m_plusStep);

        step = paramPstep - val;
        setActiveParameter(iP, val);

        for (size_t i = 0; i < nData; i++) 
        {
          jacobian.set(i,iP, 
            (m_plusStep.getCalculated(i) - m_minusStep.getCalculated(i))/step);
        }
      }
    }
}

/**
  Subclasses will use this method to declare their attributes.
  @param name :: Name of the attribute.
  @param initValue :: Initial value
  */
void IFunction::declareAttribute(const std::string &name, IFunction::Attribute initValue)
{
    if ( hasAttribute(name) )
    {
        throw std::runtime_error("Attribute "+name+" already exists.");
    }
    m_attributes[name] = initValue;
}

void IFunction::setAttribute(const std::string& attName,const IFunction::Attribute& att)
{
  auto it = m_attributes.find( attName );
  if ( it == m_attributes.end() )
  {
    throw std::invalid_argument("Attribute "+attName+" not found in function "+this->name());
  }
  it->second = att;
}

} // namespace Numeric


///\endcond TEMPLATE
