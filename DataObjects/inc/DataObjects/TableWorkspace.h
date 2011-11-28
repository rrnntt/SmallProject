#ifndef DATAOBJECTS_TableWorkspace_H
#define DATAOBJECTS_TableWorkspace_H

#include "DataObjects/DllExport.h"
#include "DataObjects/Column.h"
#include "API/Workspace.h"
#include "Kernel/Logger.h"

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

namespace DataObjects
{

  class DATAOBJECTS_EXPORT TableWorkspace: public API::Workspace
  {
  public:

    TableWorkspace();
    bool addColumn(const std::string& type, const std::string& name);
    /// Removes a column.
    void removeColumn( const std::string& name);
    /// Number of columns in the workspace.
    int columnCount() const {return static_cast<int>(m_columns.size());}
    /// Gets the shared pointer to a column.
    Column_ptr getColumn(const std::string& name);
    Column_ptr getColumn(const std::string& name)const;
    /// Gets the shared pointer to a column.
    Column_ptr getColumn(int index);
    Column_ptr getColumn(int index)const;
    /// Returns a vector of all column names.
    std::vector<std::string> getColumnNames();
    /// Number of rows in the workspace.
    int rowCount() const {return m_rowCount;}
    /// Resizes the workspace.
    void setRowCount(int count);
    /// Inserts a row before row pointed to by index and fills it with default vales.
    int insertRow(int index);
    /// Delets a row if it exists.
    void removeRow(int index);

  private:

    /**  Resize a column.
         @param c Pointer to the column
         @param size New column size
     */
    void resizeColumn(Column* c,int size)
    {
      c->resize(size);
    }

    /**  Insert a new element into a column.
         @param c Pointer to the column
         @param index Index in the column before which a new element wil be inserted.
     */
    void insertInColumn(Column* c,int index)
    {
      c->insert(index);
    }

    /**  Remove an element from a column.
         @param c Pointer to the column
         @param index Index of the element to be removed.
     */
    void removeFromColumn(Column* c,int index)
    {
      c->remove(index);
    }

    /// Used in std::find_if algorithm to find a Column with name \a name.
    class FindName
    {
      std::string m_name;///< Name to find
    public:
      /// Constructor
      FindName(const std::string& name):m_name(name){}
      /// Comparison operator
      bool operator()(const boost::shared_ptr<Column>& cp)
      {
        return cp->name() == m_name;
      }
    };


    typedef std::vector< boost::shared_ptr<Column> >::iterator column_it;///< Column iterator
    typedef std::vector< boost::shared_ptr<Column> >::const_iterator column_const_it;///< Column const iterator
    /// Shared pointers to the columns.
    std::vector< boost::shared_ptr<Column> > m_columns;
    /// row count
    int m_rowCount;
    /// Logger
    static Kernel::Logger& g_log;
  };

} // DataObjects

#endif // DATAOBJECTS_TableWorkspace_H
