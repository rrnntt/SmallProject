#ifndef DATAOBJECTS_STRINGCOLUMN_H
#define DATAOBJECTS_STRINGCOLUMN_H

#include "DataObjects/DllExport.h"
#include "DataObjects/TableColumn.h"

#include <string>

namespace DataObjects
{

  /** \class StringColumn
     Column implementation for std::string.

   */

  class StringColumn: public TableColumn<std::string>
  {
  public:
    StringColumn():TableColumn<std::string>(){}
    void read(std::istream& s, int index);
    /// Save into a file
    virtual void saveAsci(std::ostream& s, int index) const;

    /// Read from a file
    virtual void loadAsci(std::istream& s, int index);

    /// Read value from a string
    virtual void fromString(const std::string& str,size_t i);

  };
} // DataObjects


#endif // DATAOBJECTS_STRINGCOLUMN_H

