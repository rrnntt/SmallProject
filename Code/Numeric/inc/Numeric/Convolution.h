#ifndef NUMERIC_CONVOLUTION_H
#define NUMERIC_CONVOLUTION_H

#include "Numeric/DllExport.h"
#include "API/Algorithm.h"

namespace Numeric
{

class NUMERIC_EXPORT Convolution: public API::Algorithm
{
public:
  Convolution();
  virtual std::string name() const {return "Convolution";}
protected:
  virtual void exec();
};

} // NUMERIC

#endif // NUMERIC_CONVOLUTION_H
