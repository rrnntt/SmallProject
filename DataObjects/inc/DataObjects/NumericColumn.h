#ifndef DATAOBJECTS_NUMERICCOLUMN_H
#define DATAOBJECTS_NUMERICCOLUMN_H

#include "DataObjects/DllExport.h"
#include "DataObjects/Column.h"

namespace DataObjects
{

class DATAOBJECTS_EXPORT NumericColumn
{
public:
  virtual double getDouble(int index)const = 0;
};

} // DataObjects

#endif // DATAOBJECTS_NUMERICCOLUMN_H
