#ifndef NUMERIC_FUNCTIONPROPERTY_H
#define NUMERIC_FUNCTIONPROPERTY_H

#include "Numeric/DllExport.h"
#include "Numeric/IFunction.h"
#include "Kernel/Property.h"

namespace Numeric
{

class NUMERIC_EXPORT FunctionProperty: public Kernel::PropertyType<IFunction_sptr>
{
public:
    FunctionProperty():PropertyType<IFunction_sptr>(){}
    FunctionProperty(const IFunction_sptr& value):PropertyType<IFunction_sptr>(value){}
    FunctionProperty(const Property& prop);
    virtual Property& operator=(const std::string& str);
    virtual operator std::string() const;
    virtual operator IFunction_sptr() const {return m_value;}
    virtual Property& operator=(const IFunction_sptr& value) {m_value = value; return *this;}
};

} // namespace Numeric
#endif // NUMERIC_FUNCTIONPROPERTY_H
