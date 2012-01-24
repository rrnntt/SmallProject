#include "DataObjects/TableWorkspace.h"
#include "DataObjects/TableColumn.h"
#include "API/WorkspaceFactory.h"
#include "Kernel/EParser.h"
#include "Formula/Expression.h"
#include "Formula/Scalar.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

DECLARE_NUMERICTABLECOLUMN(int,int);
DECLARE_NUMERICTABLECOLUMN(double,double);
DECLARE_NUMERICTABLECOLUMN(float,float);

namespace DataObjects
{

DECLARE_WORKSPACE(TableWorkspace);

Kernel::Logger& TableWorkspace::g_log(Kernel::Logger::get("TableWorkspace"));

TableWorkspace::TableWorkspace():
m_rowCount(0)
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
        Column* c = ColumnFactory::instance().createColumn(type);
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
        throw ColumnNotFound(str);
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
        throw ColumnNotFound(str);
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

int TableWorkspace::appendRow()
{
  return insertRow(rowCount() - 1);
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

/**
 * Removes a number of rows simultaneously.
 * @param index :: Index of the first row to delete
 * @param count :: number of rows to delete
 */
void TableWorkspace::removeRows(int index, size_t count)
{
  if (count == 0) return;
  if (index >= rowCount() || index + count > rowCount())
  {
    throw std::range_error("TableWorkspace row index out of range");
  }
  for(int i = index + count - 1;i >= index; --i)
  {
    removeRow(i);
  }
}

std::vector<std::string> TableWorkspace::getColumnNames()
{
    std::vector<std::string> nameList;
    nameList.reserve(m_columns.size());
    for(column_it ci=m_columns.begin();ci!=m_columns.end();ci++)
        nameList.push_back((*ci)->name());
    return nameList;
}

/**
 * Save into a file.
 * @param fileName :: The file name.
 * @param sep :: Column separator.
 */
void TableWorkspace::saveAscii(const std::string& fileName, const std::string& sep) const
{
  std::ofstream fil(fileName.c_str());
  fil << "#sep=" << sep << std::endl;
  for(int j = 0; j < columnCount(); ++j)
  {
    // save column type and name
    Column_ptr col = getColumn(j);
    fil << '#' << col->type() << ':' << col->name() << std::endl;
  }
  for(int i = 0; i < rowCount(); ++i)
  {
    for(int j = 0; j < columnCount(); ++j)
    {
      if (j > 0) fil << sep;
      m_columns[j]->saveAsci(fil,i);
    }
    fil << std::endl;
  }
  fil.close();
}

void TableWorkspace::loadAscii(const std::string& fileName)
{
  std::ifstream fil(fileName.c_str());
  if (!fil) return;

  std::string sep; // column separator
  bool doneHeader = false; // done reading the header
  size_t row = 0; // current row index
  size_t n; // number of columns

  Kernel::SeqParser dataParser;

  std::string str;
  while(std::getline(fil,str))
  {
    if (str.empty()) continue;
    if (str[0] == '#')
    {// comments and header info
      if (doneHeader) continue; // this can only be a comment

      // parse column structure descriptor
      Kernel::SeqParser* columnParser = new Kernel::SeqParser();
      columnParser->addParser(new Kernel::WordParser(":"));
      columnParser->addParser(new Kernel::CharParser(':'));
      columnParser->addParser(new Kernel::WordParser);

      // separator parser
      Kernel::SeqParser* sepParser = new Kernel::SeqParser();
      sepParser->addParser(new Kernel::StringParser("sep="));
      sepParser->addParser(new Kernel::WordParser);

      // parse info fileds (after #)
      Kernel::AltParser infoParser;
      infoParser.addParser(sepParser);
      infoParser.addParser(columnParser);
      
      infoParser.match(str,1);

      if (!infoParser.hasMatch()) continue;

      Kernel::SeqParser* seq = dynamic_cast<Kernel::SeqParser*>(infoParser.getParser());

      if (!seq) continue; // ??

      if (seq == sepParser)
      {// reading the separator
        sep = seq->getParser(1)->match();
      }
      else if (seq == columnParser)
      {// reading column descriptor
        std::string type = seq->getParser(0)->match();
        std::string name = seq->getParser(2)->match();
        this->addColumn(type,name);
      }

    }
    else
    {// column data
      if (!doneHeader)
      {
        doneHeader = true;
        n = columnCount();
        if (n == 0) throw std::runtime_error("No columns created");
        for(size_t col = 0; col < n - 1; ++col)
        {
          dataParser.addParser( new Kernel::NotStringParser(sep));// TODO: replace with custom parser
          dataParser.addParser( new Kernel::StringParser(sep));
        }
        dataParser.addParser( new Kernel::AllParser);
      }
      dataParser.match(str);
      if (dataParser.hasMatch())
      {
        insertRow(row);
        for(size_t col = 0; col < n; ++col)
        {
          std::istringstream istr(dataParser.getParser(col*2)->match());
          m_columns[col]->read(istr,row);
        }
        ++row;
      }
    }// end column data
  }// end getline
}

/**
 * Fill numeric column with data using Formula::Expression
 * @param colName :: Column name. Must be numeric
 * @param expr :: Expression to calculate the values
 */
void TableWorkspace::fillColumn(const std::string& colName,const std::string& expr)
{
  Formula::Namespace_ptr ns(new Formula::Namespace);
  double row = 0.0;
  ns->addVariable(Formula::Variable_ptr(new Formula::Scalar(&row)),"row");
  ns->addVariable(Formula::Variable_ptr(new Formula::Scalar(&row)),"i");

  boost::shared_ptr<NumericColumn> numColumn = boost::dynamic_pointer_cast<NumericColumn>(
    getColumn(colName)
    );
  if ( !numColumn)
  {
    std::cerr << "Cannot fill non-numeric column using Expression" << std::endl;
    return;
  }

  try
  {
    Formula::Expression e(ns,expr);
    size_t n = rowCount();
    for(size_t i = 0; i < n; ++i)
    {
      row = static_cast<double>(i);
      double value = e.eval().as<Formula::Scalar>();
      numColumn->setDouble(i,value);
    }
  }
  catch(std::exception& ex)
  {
    std::cerr << "Expression failed: " << ex.what() << std::endl;
  }
}

} // DataObjects
