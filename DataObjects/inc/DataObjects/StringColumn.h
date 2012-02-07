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
    //void read(std::istream& s, int index);
    ///// Save into a file
    //virtual void saveAscii(std::ostream& s, int index) const;

    ///// Read from a file
    //virtual void loadAscii(std::istream& s, int index);

    /// Return value of a cell as a string
    virtual std::string asString(size_t i) const;
    /// Read value from a string
    virtual void fromString(const std::string& str,size_t i);

  };
} // DataObjects


#endif // DATAOBJECTS_STRINGCOLUMN_H

