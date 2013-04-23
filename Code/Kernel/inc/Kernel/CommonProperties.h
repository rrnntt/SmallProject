#ifndef KERNEL_COMMONPROPERTY_H_
#define KERNEL_COMMONPROPERTY_H_

//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Kernel/DllExport.h"
#include "Kernel/Property.h"

#include <sstream>
#include <vector>

namespace Kernel
{

  class KERNEL_EXPORT DoubleProperty: public PropertyType<double>
  {
  public:
    DoubleProperty():PropertyType<double>(){}
    DoubleProperty(const double& value):PropertyType<double>(value){}
    DoubleProperty(const Property& prop);
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
    IntProperty():PropertyType<int>(){}
    IntProperty(const int& value):PropertyType<int>(value){}
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
    BoolProperty():PropertyType<bool>(){}
    BoolProperty(const bool& value):PropertyType<bool>(value){}
    virtual Property& operator=(const std::string& str);
    virtual operator std::string() const;
    virtual Property& operator=(const bool& value) {m_value = value; return *this;}
    virtual operator bool() const {return m_value;}
//  protected:
//    bool m_value;
  };

  class KERNEL_EXPORT StringProperty: public Property
  {
  public:
    StringProperty() : m_value() {}
    StringProperty(const std::string& value) : m_value(value) {}
    StringProperty(const std::vector<std::string>& values, const std::string& defValue = "");
    virtual Property& operator=(const std::string& str);
    virtual operator std::string() const;
    std::vector<std::string> getValues() const {return m_values;}
  protected:
    std::string m_value;
    std::vector<std::string> m_values;
  };

} // namespace Kernel

#endif //KERNEL_COMMONPROPERTY_H_
