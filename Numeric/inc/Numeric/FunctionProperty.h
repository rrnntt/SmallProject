#ifndef NUMERIC_FUNCTIONPROPERTY_H
#define NUMERIC_FUNCTIONPROPERTY_H

#include "Numeric/DllExport.h"
#include "Numeric/IFunction.h"
#include "Kernel/Property.h"

namespace Numeric
{

class NUMERIC_EXPORT FunctionProperty: public Kernel::PropertyType<IFunction_ptr>
{
public:
    FunctionProperty():PropertyType<IFunction_ptr>(){}
    FunctionProperty(const IFunction_ptr& value):PropertyType<IFunction_ptr>(value){}
    FunctionProperty(const Property& prop);
    virtual Property& operator=(const std::string& str);
    virtual operator std::string() const;
    virtual operator IFunction_ptr() const {return m_value;}
    virtual Property& operator=(const IFunction_ptr& value) {m_value = value; return *this;}
};

} // namespace Numeric
#endif // NUMERIC_FUNCTIONPROPERTY_H
