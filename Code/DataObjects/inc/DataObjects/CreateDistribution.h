#ifndef DATAOBJECTS_CREATEDISTRIBUTION_H
#define DATAOBJECTS_CREATEDISTRIBUTION_H

#include "DataObjects/DllExport.h"
#include "API/Algorithm.h"

namespace DataObjects
{

  /** 
   * Fill a column in a TableWorkspace with random values 
   */

  class CreateDistribution: public API::Algorithm
  {
  public:
    CreateDistribution();
    virtual std::string name() const {return "CreateDistribution";}

  protected:
    void exec();
  };
} // DataObjects


#endif // DATAOBJECTS_CREATEDISTRIBUTION_H

