#ifndef KERNEL_COMMONPROPERTY_H_
#define KERNEL_COMMONPROPERTY_H_

//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Kernel/DllExport.h"
#include "Kernel/Property.h"

#include <sstream>

namespace Kernel
{

  class KERNEL_EXPORT DoubleProperty: public PropertyType<double>
  {
  public:
    virtual Property& operator=(const std::string& str);
    virtual operator std::string() const;
    virtual operator double() const {return m_value;}
    virtual Property& operator=(const double& value) {m_value = value; return *this;}
//  protected:
//    double m_value;
  };

  class KERNEL_EXPORT IntProperty: public PropertyType<int>
  {
  public:
    virtual Property& operator=(const std::string& str);
    virtual operator std::string() const;
    virtual Property& operator=(const int& value) {m_value = value; return *this;}
    virtual operator int() const {return m_value;}
//  protected:
//    int m_value;
  };

  class KERNEL_EXPORT BoolProperty: public PropertyType<bool>
  {
  public:
    virtual Property& operator=(const std::string& str);
    virtual operator std::string() const;
    virtual Property& operator=(const bool& value) {m_value = value; return *this;}
    virtual operator bool() const {return m_value;}
   // Default constructor
//  protected:
//    bool m_value;
  };

  class KERNEL_EXPORT StringProperty: public Property
  {
  public:
    virtual Property& operator=(const std::string& str);
    virtual operator std::string() const;
    // Default constructor
    StringProperty() : m_value() {}
    StringProperty(const std::string& value) : m_value(value) {}
  protected:
    std::string m_value;
  };

} // namespace Kernel

#endif //KERNEL_COMMONPROPERTY_H_
