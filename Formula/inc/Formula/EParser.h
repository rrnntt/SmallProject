#ifndef FORMULA_EPARSER_H
#define FORMULA_EPARSER_H

#include "Formula/DllExport.h"
#include "Formula/Operators.h"

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>
#include <map>
#include <set>

namespace Formula
{

class FORMULA_EXPORT EParser
{
public:
    EParser();
    void parse(const std::string& str);
private:

    std::string m_funct; ///< Function name
    std::string m_op;    ///< Operator connecting this expression to its sibling on the left
    std::vector<EParser> m_terms;///< Child expressions (function arguments)

    Operators_ptr m_operators;

};

} // Formula

#endif // FORMULA_EPARSER_H
