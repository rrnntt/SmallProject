#ifndef DATAOBJECTS_CALCULATECOLUMNVALUES_H
#define DATAOBJECTS_CALCULATECOLUMNVALUES_H

#include "DataObjects/DllExport.h"
#include "API/Algorithm.h"

namespace DataObjects
{

  class TableWorkspace;

class DATAOBJECTS_EXPORT CalculateColumnValues: public API::Algorithm
{
public:
  CalculateColumnValues();
  ~CalculateColumnValues(){}
  virtual std::string name() const {return "CalculateColumnValues";}
protected:
  virtual void exec();
  boost::shared_ptr<TableWorkspace> getTableWorkspace();
};

} // namespace DataObjects
#endif // DATAOBJECTS_CALCULATECOLUMNVALUES_H
