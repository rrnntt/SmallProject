#include "DataObjects/TableWorkspace.h"
#include "DataObjects/TableColumn.h"
#include "API/WorkspaceFactory.h"

#include <iostream>
#include <string>
#include <sstream>

DECLARE_NUMERICTABLECOLUMN(int,int);
DECLARE_NUMERICTABLECOLUMN(double,double);
DECLARE_TABLECOLUMN(std::string,string);

namespace DataObjects
{

DECLARE_WORKSPACE(TableWorkspace);

Kernel::Logger& TableWorkspace::g_log(Kernel::Logger::get("TableWorkspace"));

TableWorkspace::TableWorkspace()
{
  std::cerr<<"TableWorkspace\n";
}

/** @param type Data type of the column.
    @param name Column name.
    @return True if the column was successfully created.
*/
bool TableWorkspace::addColumn(const std::string& type, const std::string& name)
{
    if (type.empty())
    {
        g_log.error("Empty string passed as type argument of createColumn.");
        return false;
    }
    if (name.empty())
    {
        g_log.error("Empty string passed as name argument of createColumn.");
        return false;
    }
    // Check that there is no column with the same name.
    column_it ci = std::find_if(m_columns.begin(),m_columns.end(),FindName(name));
    if (ci != m_columns.end())
        {
            g_log.error("Column with name "+name+" already exists.\n");
            return false;
        }
    try
    {
        Column* c = ColumnFactory::instance().create(type);
        m_columns.push_back(boost::shared_ptr<Column>(c));
        c->setName(name);
        resizeColumn(c,rowCount());
    }
    catch(std::exception& e)
    {
        g_log.error("Column of type "+type+" and name "+name+" has not been created.\n");
        g_log.error(e.what());
        return false;
    }
    return true;
}

/** If count is greater than the current number of rows extra rows are added to the bottom of the table.
    Otherwise rows at the end are erased to reach the new size.
    @param count New number of rows.
*/
void TableWorkspace::setRowCount(int count)
{
    if (count == rowCount()) return;
    for(column_it ci=m_columns.begin();ci!=m_columns.end();ci++)
        resizeColumn(ci->get(),count);
    m_rowCount = count;
}

/// Gets the shared pointer to a column.
Column_ptr TableWorkspace::getColumn(const std::string& name)
{
    column_it ci = std::find_if(m_columns.begin(),m_columns.end(),FindName(name));
    if (ci == m_columns.end())
    {
        std::string str = "Column " + name + " does not exist.\n";
        g_log.error(str);
        throw std::runtime_error(str);
    }
    return *ci;
}

/// Gets the shared pointer to a column.
Column_ptr TableWorkspace::getColumn(const std::string& name)const
{
    column_const_it ci = std::find_if(m_columns.begin(),m_columns.end(),FindName(name));
    if (ci == m_columns.end())
    {
        std::string str = "Column " + name + " does not exist.\n";
        g_log.error(str);
        throw std::runtime_error(str);
    }
    return *ci;
}

/// Gets the shared pointer to a column.
Column_ptr TableWorkspace::getColumn(int index)
{
    if (index >= columnCount())
    {
        std::ostringstream str;
        str << "Column index is out of range: "<<index<<"("<<columnCount()<<")\n";
        g_log.error(str.str());
        throw std::range_error(str.str());
    }
    return m_columns[index];
}

/// Gets the shared pointer to a column.
Column_ptr TableWorkspace::getColumn(int index)const
{
    if (index >= columnCount())
    {
        std::ostringstream str;
        str << "Column index is out of range: "<<index<<"("<<columnCount()<<")\n";
        g_log.error(str.str());
        throw std::range_error(str.str());
    }
    return m_columns[index];
}

void TableWorkspace::removeColumn( const std::string& name)
{
    column_it ci = std::find_if(m_columns.begin(),m_columns.end(),FindName(name));
    if (ci != m_columns.end())
    {
        if ( !ci->unique() )
        {
            g_log.error("Deleting column in use ("+name+").\n");
        }
        m_columns.erase(ci);
    }
}

/** @param index Points where to insert the new row.
    @return Position of the inserted row.
*/
int TableWorkspace::insertRow(int index)
{
    if (index >= rowCount()) index = rowCount();
    for(column_it ci=m_columns.begin();ci!=m_columns.end();ci++)
        insertInColumn(ci->get(),index);
    ++m_rowCount;
    return index;
}

/** @param index Row to delete.
*/
void TableWorkspace::removeRow(int index)
{
    if (index >= rowCount())
    {
      std::ostringstream ostr;
      ostr << "Attempt to delete a non-existing row ("<<index<<")\n";
        g_log.error(ostr.str());
        return;
    }
    for(column_it ci=m_columns.begin();ci!=m_columns.end();ci++)
        removeFromColumn(ci->get(),index);
    --m_rowCount;
}

std::vector<std::string> TableWorkspace::getColumnNames()
{
    std::vector<std::string> nameList;
    nameList.reserve(m_columns.size());
    for(column_it ci=m_columns.begin();ci!=m_columns.end();ci++)
        nameList.push_back((*ci)->name());
    return nameList;
}

} // DataObjects
