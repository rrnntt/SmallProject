#ifndef NUMERIC_ANALYSEBASIS_H
#define NUMERIC_ANALYSEBASIS_H

#include "Numeric/DllExport.h"
#include "API/Algorithm.h"

namespace Numeric
{

class NUMERIC_EXPORT AnalyseBasis: public API::Algorithm
{
public:
  AnalyseBasis();
  virtual std::string name() const {return "AnalyseBasis";}
protected:
  virtual void exec();
};

} // NUMERIC

#endif // NUMERIC_ANALYSEBASIS_H
