#ifndef DATAOBJECTS_CALCULATECOLUMNVALUES_H
#define DATAOBJECTS_CALCULATECOLUMNVALUES_H

#include "DataObjects/DllExport.h"
#include "API/Algorithm.h"

namespace API
{
  class TableWorkspace;
}

namespace DataObjects
{

class DATAOBJECTS_EXPORT CalculateColumnValues: public API::Algorithm
{
public:
  CalculateColumnValues();
  ~CalculateColumnValues(){}
  virtual std::string name() const {return "CalculateColumnValues";}
protected:
  virtual void exec();
  boost::shared_ptr<API::TableWorkspace> getTableWorkspace();
};

} // namespace DataObjects
#endif // DATAOBJECTS_CALCULATECOLUMNVALUES_H
