#ifndef NUMERIC_FIT_H
#define NUMERIC_FIT_H

#include "Numeric/DllExport.h"
#include "API/Algorithm.h"

namespace Numeric
{

class NUMERIC_EXPORT Fit: public API::Algorithm
{
public:
  Fit();
  virtual std::string name() const {return "Fit";}
protected:
  virtual void exec();
  void applyProperty(const std::string& name);
private:
  bool m_functionSet;
  bool m_workspaceSet;
};

} // NUMERIC

#endif // NUMERIC_FIT_H
