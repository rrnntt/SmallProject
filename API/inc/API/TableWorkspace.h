#ifndef API_TableWorkspace_H
#define API_TableWorkspace_H

#include "API/DllExport.h"
#include "API/Column.h"
#include "API/Workspace.h"
#include "Kernel/Logger.h"

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>
#include <stdexcept>

namespace API
{

  class API_EXPORT TableWorkspace: public API::Workspace
  {
  public:

    // copied from qwt_plot_curve.h
    enum CurveStyle
    {
        NoCurve,
        Lines,
        Sticks,
        Steps,
        Dots,
    };

    class ColumnNotFound: public std::runtime_error
    {
    public:
      ColumnNotFound(const std::string mess):std::runtime_error(mess){}
    };

    TableWorkspace();
    virtual std::string id()const {return "TableWorkspace";}
    Column_ptr addColumn(const std::string& type, const std::string& name);
    /// Removes a column.
    void removeColumn( const std::string& name);
    /// Number of columns in the workspace.
    size_t columnCount() const {return m_columns.size();}
    /// Gets the shared pointer to a column.
    Column_ptr getColumn(const std::string& name);
    Column_ptr getColumn(const std::string& name)const;
    /// Gets the shared pointer to a column.
    Column_ptr getColumn(int index);
    Column_ptr getColumn(int index)const;
    Column_ptr getOrAddColumn(const std::string& type, const std::string& name);
    /// Returns a vector of all column names.
    std::vector<std::string> getColumnNames();
    /// Number of rows in the workspace.
    size_t rowCount() const {return m_rowCount;}
    /// Resizes the workspace.
    void setRowCount(int count);
    /// Inserts a row before row pointed to by index and fills it with default vales.
    int insertRow(int index);
    /// Appends a row and fills it with default vales.
    int appendRow();
    /// Delets a row if it exists.
    void removeRow(int index);
    /// Delets several rows if they exist.
    void removeRows(int index, size_t count);
    /// Save into an asci file
    virtual void saveAscii(const std::string& fileName, const std::string& sep = "") const;
    /// Load from an asci file
    virtual void loadAscii(const std::string& fileName);
    /// Fill numeric column with data using Formula::Expression
    //void fillColumn(const std::string& colName,const std::string& expr);
    /// Check if a column exists
    bool hasColumn(const std::string& colName) const;
    /// Remove all columns
    void removeAllColumns();
    int getCurveStyle() const {return m_defaultCurveStyle;}
    void setCurveStyle(int s) {m_defaultCurveStyle = s;}
    /// Add a column of type "double".
    Column_ptr addDoubleColumn(const std::string& name, int plotRole = 0);
    /// Return reference to the data of a "double" column
    std::vector<double>& getDoubleData(const std::string& name) ;

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
    size_t m_rowCount;
    std::string m_defaultSeparator; ///< default separator for saving in ascii files
    int m_defaultCurveStyle;///< 0-NoCurve, 1-Lines, 2-Sticks, 3-Steps, 4-Dots (must be the same as in qwt_plot_curve.h)

    /// Logger
    static Kernel::Logger& g_log;
  };

  typedef boost::shared_ptr<TableWorkspace> TableWorkspace_ptr;

} // API

#endif // API_TableWorkspace_H
