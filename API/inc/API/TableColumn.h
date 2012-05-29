#ifndef API_TABLECOLUMN_H
#define API_TABLECOLUMN_H

#include "API/DllExport.h"
#include "API/Column.h"
#include "API/NumericColumn.h"
#include "API/ColumnFactory.h"
#include "Kernel/Logger.h"

#include <boost/lexical_cast.hpp>

#include <vector>
#include <typeinfo>
#include <stdexcept>


namespace API
{

  template< class T>
  class TableVector;

  /** \class TableColumn

    Class TableColumn implements abstract class Column for any copyable data type.
    A TableColumn is created using TableWorkspace::addColumn(type,name).
    type is the simbolic name of the data type which must be first declared with
    DECLARE_TABLECOLUMN macro. Predeclared types are:

    "int"    for int
    "float"  for float
    "double" for double
    "bool"   for Boolean
    "str"    for std::string
    "V3D"    for Mantid::Geometry::V3D

    Boolean is used instead of bool because of bool's non-standard treatmemt in std::vector.

*/
  template <class Type>
  class TableColumn: public Column
  {
  public:

    //TableColumn();
    /// Virtual destructor.
    virtual ~TableColumn(){}
    /// Number of individual elements in the column.
    int size()const{return int(m_data.size());}
    /// Type id of the data in the column
    const std::type_info& get_type_info()const{return typeid(Type);}
    /// Type id of the pointer to data in the column
    const std::type_info& get_pointer_type_info()const{return typeid(Type*);}
    /// Return value of a cell as a string
    virtual std::string asString(size_t i) const
    {
      return boost::lexical_cast<std::string>(m_data[i]);
    }
    /// Read value from a string
    virtual void fromString(const std::string& str,size_t i)
    {
      m_data[i] = boost::lexical_cast<Type>(str);
    }

    ///// Output to an ostream.
    //void print(std::ostream& s, int index)const{s << m_data[index];}
    //void read(std::istream& s, int index) {s >> m_data[index];}
    /// Type check
    bool isBool()const{return typeid(Type) == typeid(Boolean);}
    /// Memory used by the column
    long int sizeOfData()const{return static_cast<long int>(m_data.size()*sizeof(Type));}

    /// Reference to the data.
    std::vector<Type>& data(){return m_data;}
    const std::vector<Type>& data()const{return m_data;}
  protected:
    /// Resize.
    void resize(int count){m_data.resize(count);}
    /// Inserts default value at position index.
    void insert(int index)
    {
      if (index < int(m_data.size()))
        m_data.insert(m_data.begin()+index,Type());
      else
        m_data.push_back(Type());
    }
    /// Removes an item at index.
    void remove(int index){m_data.erase(m_data.begin()+index);}
    /// Returns a pointer to the data element.
    void* void_pointer(int index){return &m_data[index];}
    /// Column data
    std::vector<Type> m_data;
    friend class TableWorkspace;
    template <typename T> friend class ColumnVector;
  };

  /// Shared pointer to a column with aoutomatic type cast and data type check.
  /// Can be created with TableWorkspace::getColumn(...)
  template< class T>
  class TableColumn_ptr: public boost::shared_ptr<TableColumn<T> >
  {
  public:
    /** Constructor
        @param c Shared pointer to a column
      */
    TableColumn_ptr(boost::shared_ptr<Column> c):boost::shared_ptr<TableColumn<T> >(boost::dynamic_pointer_cast<TableColumn<T> >(c))
    {
      if (this->get() == NULL)
      {
        Kernel::Logger& log = Kernel::Logger::get("TableWorkspace");
        std::string str = "Data type of column "+c->name()+" does not match "+typeid(T).name();
        log.error(str);
        throw std::runtime_error(str);
      }
    }
  };

  /// Special case of bool
  template<>
  class TableColumn_ptr<bool>: public TableColumn_ptr<Boolean>
  {
  public:
    /** Constructor
        @param c Shared pointer to a column
      */
    TableColumn_ptr(boost::shared_ptr<Column> c):TableColumn_ptr<Boolean>(c)
    {
      if (this->get() == NULL)
      {
        Kernel::Logger& log = Kernel::Logger::get("TableWorkspace");
        std::string str = "Data type of column "+c->name()+" does not match "+typeid(Boolean).name();
        log.error(str);
        throw std::runtime_error(str);
      }
    }
  };

  template <class Type>
  class NumericTableColumn: public TableColumn<Type>, public NumericColumn
  {
  public:

    /// Virtual destructor.
    virtual ~NumericTableColumn(){}

    double getDouble(int index) const
    {
      return double(TableColumn<Type>::data()[index]);
    }
    void setDouble(int index, double value)
    {
      TableColumn<Type>::data()[index] = static_cast<Type>(value);
    }
  virtual std::vector<double>* getDoubleVector() {return nullptr;}
  virtual std::vector<float>* getFloatVector() {return nullptr;}
  virtual std::vector<int>* getIntVector() {return nullptr;}
  virtual const std::vector<double>* getDoubleVector() const {return nullptr;}
  virtual const std::vector<float>* getFloatVector() const {return nullptr;}
  virtual const std::vector<int>* getIntVector() const {return nullptr;}
  };

  template<>
  const std::vector<double>* NumericTableColumn<double>::getDoubleVector() const {return &m_data;}

  template<>
  const std::vector<float>* NumericTableColumn<float>::getFloatVector() const {return &m_data;}

  template<>
  const std::vector<int>* NumericTableColumn<int>::getIntVector() const {return &m_data;}

  template<>
  std::vector<double>* NumericTableColumn<double>::getDoubleVector() {return &m_data;}

  template<>
  std::vector<float>* NumericTableColumn<float>::getFloatVector() {return &m_data;}

  template<>
  std::vector<int>* NumericTableColumn<int>::getIntVector() {return &m_data;}

} // DataObjects

/*
    Macro to declare a type to be used with TableColumn.
    DataType is the actual C++ type. TypeName is a symbolic name, used in TableWorkspace::createColumn(...)
    TypeName can contain only letters, numbers and _s.
*/
#define DECLARE_TABLECOLUMN(DataType,TypeName) \
    namespace{ \
    int register_table_column_##TypeName(  \
    (API::ColumnFactory::instance().subscribe< API::TableColumn< DataType > >(#TypeName),0)); \
    }

#define DECLARE_NUMERICTABLECOLUMN(DataType,TypeName) \
    namespace{ \
    int register_numeric_table_column_##TypeName(  \
    (API::ColumnFactory::instance().subscribe< API::NumericTableColumn< DataType > >(#TypeName),0)); \
    }

#endif // API_TABLECOLUMN_H

