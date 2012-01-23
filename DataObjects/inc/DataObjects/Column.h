#ifndef DATOBJECTS_COLUMN_H
#define DATOBJECTS_COLUMN_H

#include "DataObjects/DllExport.h"

#include <ostream>
#include <istream>
#include <string>
#include <boost/shared_ptr.hpp>

namespace DataObjects
{

class NumericColumn;

class DATAOBJECTS_EXPORT Column
{
public:
    /// Virtual destructor
    virtual ~Column() {}

    /// Name (caption) of the column.
    const std::string& name()const{return m_name;}

    /// Type of the column data.
    const std::string& type()const{return m_type;}

    /// Renames the column.
    void setName(const std::string& str){m_name = str;}

    /// Number of individual elements in the column.
    virtual int size()const = 0;

    /// Returns typeid for the data in the column
    virtual const std::type_info& get_type_info()const = 0;

    /// Returns typeid for the pointer type to the data element in the column
    virtual const std::type_info& get_pointer_type_info()const = 0;

    /// Prints
    virtual void print(std::ostream& s, int index) const = 0;

    /// Reads
    virtual void read(std::istream& s, int index) = 0;

    /// Save into a file
    virtual void saveAsci(std::ostream& s, int index) const;

    /// Read from a file
    virtual void loadAsci(std::istream& s, int index);

    /// Return value of a cell as a string
    virtual std::string asString(size_t i) const;

    /// Read value from a string
    virtual void fromString(const std::string& str,size_t i);

    /// Templated method for returning a value. No type checks are done.
    template<class T>
    T& cell(int index)
    {
        return *static_cast<T*>(void_pointer(index));
    }

    /// Templated method for returning a value (const version). No type checks are done.
    template<class T>
    const T& cell(int index)const
    {
        return *static_cast<T*>(void_pointer(index));
    }

    /// Type check.
    template<class T>
    bool isType()const
    {
        return get_type_info() == typeid(T);
    }

    /// Specialized type check
    virtual bool isBool()const = 0;

    /// Must return overall memory size taken by the column.
    virtual long int sizeOfData()const = 0;

    bool isNumeric() const;

    NumericColumn* asNumeric();

protected:
    /// Sets the new column size.
    virtual void resize(int count) = 0;
    /// Inserts an item.
    virtual void insert(int index) = 0;
    /// Removes an item.
    virtual void remove(int index) = 0;
    /// Pointer to a data element
    virtual void* void_pointer(int index) = 0;
private:
    std::string m_name;///< name
    std::string m_type;///< type
    friend class ColumnFactory;
    friend class TableWorkspace;
    template<class T> friend class ColumnVector;
};

/**  @class Boolean
    As TableColumn stores its data in a std::vector bool type cannot be used
    in the same way as the other types. Class Boolean is used instead.
*/
struct DATAOBJECTS_EXPORT Boolean
{
    /// Default constructor
    Boolean():value(false){}
    /// Conversion from bool
    Boolean(bool b):value(b){}
    /// Returns bool
    operator bool(){return value;}
        /// equal to operator
        bool operator==(const Boolean& b)const
        {return(this->value==b.value);
        }
    bool value;///< boolean value
};

/// Printing Boolean to an output stream
DATAOBJECTS_EXPORT std::ostream& operator<<(std::ostream& ,const Boolean& );
DATAOBJECTS_EXPORT std::istream& operator>>(std::istream& ,Boolean& );

typedef boost::shared_ptr<Column> Column_ptr;

} // DataHandling

#endif // DATOBJECTS_COLUMN_H
