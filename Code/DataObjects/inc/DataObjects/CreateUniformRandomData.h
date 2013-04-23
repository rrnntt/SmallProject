#ifndef DATAOBJECTS_CREATEUNIFORMRANDOMDATA_H
#define DATAOBJECTS_CREATEUNIFORMRANDOMDATA_H

#include "DataObjects/DllExport.h"
#include "API/Algorithm.h"

namespace DataObjects
{

  /** \class StringColumn
     Column implementation for std::string.

   */

  class CreateUniformRandomData: public API::Algorithm
  {
  public:
    CreateUniformRandomData();
    virtual std::string name() const {return "CreateUniformRandomData";}

  protected:
    void exec();
  };
} // DataObjects


#endif // DATAOBJECTS_CREATEUNIFORMRANDOMDATA_H

