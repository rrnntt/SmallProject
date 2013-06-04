#ifndef NUMERIC_FIT_H
#define NUMERIC_FIT_H

#include "Numeric/DllExport.h"
#include "API/Algorithm.h"

namespace Numeric
{

//=================================================
// Forward declarations
//=================================================
class FunctionDomain;
class FunctionValues;

class NUMERIC_EXPORT Fit: public API::Algorithm
{
public:
  Fit();
  virtual std::string name() const {return "Fit";}
protected:
  virtual void exec();
  void applyProperty(const std::string& name);
  void declareMultiDomainProperties();
private:
  bool m_functionSet;
  bool m_workspaceSet;
  bool m_isMultiDomain;
};

} // NUMERIC

#endif // NUMERIC_FIT_H
