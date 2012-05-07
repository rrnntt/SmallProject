#ifndef DATAOBJECTS_CHEBFUNTOTABLE_H
#define DATAOBJECTS_CHEBFUNTOTABLE_H

#include "DataObjects/DllExport.h"

#include "API/Algorithm.h"

namespace DataObjects
{

class DATAOBJECTS_EXPORT ChebfunToTable: public API::Algorithm
{
public:
  ChebfunToTable();
  virtual std::string name() const {return "ChebfunToTable";}

protected:
  void exec();
};

} // DataObjects

#endif // DATAOBJECTS_CHEBFUNTOTABLE_H
