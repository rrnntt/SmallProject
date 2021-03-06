#include "API/TableWorkspace.h"
#include "API/TableColumn.h"
#include "API/WorkspaceFactory.h"
#include "Kernel/EParser.h"
//#include "Formula/Expression.h"
//#include "Formula/Scalar.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

namespace API
{

DECLARE_WORKSPACE(TableWorkspace);

Kernel::Logger& TableWorkspace::g_log(Kernel::Logger::get("TableWorkspace"));

TableWorkspace::TableWorkspace():
m_rowCount(0),
m_defaultSeparator(","),
m_defaultCurveStyle(Lines)
{
  std::cerr<<"TableWorkspace\n";
}

/** @param type Data type of the column.
    @param name Column name.
    @return True if the column was successfully created.
*/
Column_ptr TableWorkspace::addColumn(const std::string& type, const std::string& name)
{
  Column_ptr column;
    if (type.empty())
    {
        g_log.error("Empty string passed as type argument of createColumn.");
        return Column_ptr();
    }
    if (name.empty())
    {
        g_log.error("Empty string passed as name argument of createColumn.");
        return Column_ptr();
    }
    // Check that there is no column with the same name.
    column_it ci = std::find_if(m_columns.begin(),m_columns.end(),FindName(name));
    if (ci != m_columns.end())
        {
            g_log.error("Column with name "+name+" already exists.\n");
            return Column_ptr();
        }
    try
    {
        Column* c = ColumnFactory::instance().createColumn(type);
        column.reset( c );
        m_columns.push_back( column );
        c->setName(name);
        resizeColumn(c,rowCount());
    }
    catch(std::exception& e)
    {
        g_log.error("Column of type "+type+" and name "+name+" has not been created.\n");
        g_log.error(e.what());
        return Column_ptr();
    }
    return column;
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

/**
 * Get a column of given type and name. Ensure it exists.
 * If column exists but type is wrong throws runtime_error.
 */
Column_ptr TableWorkspace::getOrAddColumn(const std::string& type, const std::string& name)
{
  if (hasColumn(name))
  {
    if (getColumn(name)->type() != type)
    {
      throw std::runtime_error("Wrong type for the "+name+" column in "+id());
    }
  }
  else
  {
    addColumn(type,name); 
  }
  return getColumn(name);
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
  return insertRow(rowCount());
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

void TableWorkspace::removeAllColumns()
{
  m_columns.clear();
  m_rowCount = 0;
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
  std::string separator = sep.empty() ? m_defaultSeparator : sep;
  std::ofstream fil(fileName.c_str());
  fil << "#sep=" << separator << std::endl;
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
      if (j > 0) fil << separator;
      fil << m_columns[j]->asString(i);
    }
    fil << std::endl;
  }
  fil.close();
}

/**
 * Loads a TableWorkspace from an ASCII file. The data must be in columns.
 * The file structure:
 *  - header: lines start with '#' followed by one of these:
 *     sep=<sparator>
 *     <column type>:<column name>
 *   separator is a string that divides the columns. Data in the column must not contain <separator>.
 *  - the data in columns
 *  - any line starting with '#' is ignored.
 */
void TableWorkspace::loadAscii(const std::string& fileName)
{
  std::ifstream fil(fileName.c_str());
  if (!fil) return;

  removeAllColumns();
  std::string sep; // column separator
  bool doneHeader = false; // done reading the header
  size_t row = 0; // current row index
  size_t n; // number of columns

  // parser for a line of data
  Kernel::SeqParser dataParser;
  size_t firstEmptyParser = 0;
  // parsers for individual cells, size == columnCount
  // the parsers will be owned by dataParser - no need to delete explicitly
  std::vector<Kernel::IParser*> cellParsers; 

  std::string str;
  while(std::getline(fil,str))
  {
    if (str.empty()) continue;
    if (str[0] == '#')
    {// comments and header info
      if (doneHeader) continue; // this can only be a comment

      // parse column structure descriptor
      Kernel::SeqParser* columnParser = new Kernel::SeqParser();
      columnParser->addParser(new Kernel::WordParser(":"));// 0
      columnParser->addParser(new Kernel::CharParser(':'));// 1
      columnParser->addParser(new Kernel::WordParser("["));// 2
      columnParser->addParser(new Kernel::BracketParser("[","]"),'*');// 3,0

      // separator parser
      Kernel::SeqParser* sepParser = new Kernel::SeqParser();
      sepParser->addParser(new Kernel::StringParser("sep="));
      sepParser->addParser(new Kernel::AllParser);

      // parse info fileds (after #)
      Kernel::AltParser infoParser;
      infoParser.addParser(sepParser);
      infoParser.addParser(columnParser);
      
      // start parsing from the second character.
      infoParser.match(str,1);

      if (!infoParser.hasMatch()) continue;

      Kernel::SeqParser* seq = dynamic_cast<Kernel::SeqParser*>(infoParser.getParser());

      if (!seq) continue; // ??

      if (seq == sepParser)
      {// reading the separator
        sep = seq->getParser(1)->match();
        if (sep.find_first_not_of(' ') == std::string::npos)
        {// 
          sep = "";
        }
      }
      else if (seq == columnParser)
      {// reading column descriptor
        std::string type = seq->getParser(0)->match();
        std::string name = seq->getParser(2)->match();
        this->addColumn(type,name);
        // get formatter in square brackets after column name
        Kernel::BracketParser* fmtParser = seq->get<Kernel::BracketParser>(3,0);
        if (fmtParser)
        {
          if (fmtParser->hasMatch() && !fmtParser->isInnerEmpty()) // TODO: create the parser in a separate method
          {// there is something in the square brackets
            std::string formatter = fmtParser->getParser(0)->match(); // return string from the inner parser
            // interpret formatter as containing openning and closing delimiters for the column, e.g. quotes
            // in this case formatter must have even size
            size_t n2 = formatter.size() / 2;
            if (2*n2 != formatter.size())
            {
              throw std::runtime_error("TableWorkspace::loadASCII: formatter for column "+type+":"+name+
                " has wrong format:\nopenning and closing delimiters must have equal size.");
            }
            cellParsers.push_back(new Kernel::BracketParser(formatter.substr(0,n2),formatter.substr(n2)));
          }
          else
          {
            if (!sep.empty())
            {
              cellParsers.push_back(new Kernel::NotStringParser(sep));
            }
            else
            {
              cellParsers.push_back(new Kernel::NotStringParser(" "));
            }
          }
        }
        else
        {
          throw std::runtime_error("Strange error in TableWorkspace::loadASCII");
        }
      }

    }
    else
    {// column data
      if (!doneHeader)
      {
        doneHeader = true;
        n = columnCount();
        if (n == 0) throw std::runtime_error("No columns created");
        if ( sep.empty() )
        {
          dataParser.addParser( new Kernel::StringParser(" "), '*');
          firstEmptyParser = 1;
        }
        for(size_t col = 0; col < n - 1; ++col)
        {
          dataParser.addParser( cellParsers[col] ,'*');
          if (!sep.empty())
          {
            dataParser.addParser( new Kernel::StringParser(sep));
          }
          else
          {
            dataParser.addParser( new Kernel::StringParser(" "), '+');
          }
        }
        //dataParser.addParser( new Kernel::AllParser,'*');
        dataParser.addParser(cellParsers.back());
      }
      dataParser.match(str);
      if (dataParser.hasMatch())
      {
        row = appendRow();
        for(size_t col = 0; col < n; ++col)
        {
          size_t i = col*2 + firstEmptyParser;
          Kernel::BracketParser* bracketParser = col < n - 1 ?
             dataParser.get<Kernel::BracketParser>( i, 0 ) // it's in a '*' list
            :dataParser.get<Kernel::BracketParser>( i );   // last column's parser isn't in a list
          if (bracketParser)
          {
            // read from the inner parser
            //m_columns[col]->fromString(bracketParser->getParser(0)->match(),row); why doesn't it work?
            m_columns[col]->fromString(std::string(bracketParser->getInnerStart(),bracketParser->getInnerEnd()),row);
          }
          else
          {
            m_columns[col]->fromString(dataParser.getParser(i)->match(),row);
          }
        }
      }
      else
      {
        // TODO: proper logging
        std::cerr << "sep=<"<<sep<<">("<<sep.size()<<")"<<std::endl;
        for(size_t col = 0; col < n - 1; ++col)
        {
          std::cerr << "Parser " << col << " " << dataParser.getParser(col*2)->hasMatch() << std::endl;
        }
        throw std::runtime_error("Could not read row "+boost::lexical_cast<std::string>(row)+": \n"+str);
      }
    }// end column data
  }// end getline
  // file's separator becomes default for this workspace
  m_defaultSeparator = sep;
}

/**
 * Fill numeric column with data using Formula::Expression
 * @param colName :: Column name. Must be numeric
 * @param expr :: Expression to calculate the values
 */
//void TableWorkspace::fillColumn(const std::string& colName,const std::string& expr)
//{
//  // exception is thrown if the column doesn't exist
//  boost::shared_ptr<NumericColumn> numColumn = boost::dynamic_pointer_cast<NumericColumn>(
//    getColumn(colName)
//    );
//  if ( !numColumn)
//  {
//    throw std::runtime_error("Cannot fill non-numeric column using Expression");
//  }
//
//  Formula::Namespace_ptr ns(new Formula::Namespace);
//  double row = 0.0;
//  ns->addVariable(Formula::Variable_ptr(new Formula::Scalar(&row)),"row");
//  ns->addVariable(Formula::Variable_ptr(new Formula::Scalar(&row)),"i");
//  ns->addVariable(Formula::Variable_ptr(new Formula::Scalar(2*acos(0.0))),"pi");
//  ns->addVariable(Formula::Variable_ptr(new Formula::Scalar(exp(1.0))),"e");
//  
//  // define vars referencing values in all num columns
//  std::vector<double> columnVars(columnCount());
//  std::vector<size_t> columnIndex;
//  for(auto col = m_columns.begin(); col != m_columns.end(); ++col)
//  {
//    boost::shared_ptr<NumericColumn> nc = boost::dynamic_pointer_cast<NumericColumn>(*col);
//    if (nc)
//    {
//      size_t i = static_cast<size_t>(col - m_columns.begin());
//      ns->addVariable(Formula::Variable_ptr(new Formula::Scalar(&columnVars[i])),(**col).name());
//      columnIndex.push_back(i);
//    }
//  }
//
//  try
//  {
//    Formula::Expression e(ns,expr);
//    size_t n = rowCount();
//    for(size_t i = 0; i < n; ++i)
//    {
//      row = static_cast<double>(i);
//      // update the variables
//      for(auto col = columnIndex.begin(); col != columnIndex.end(); ++col)
//      {
//        columnVars[*col] = getColumn(*col)->asNumeric()->getDouble(row);
//      }
//      double value = e.eval().as<Formula::Scalar>();
//      numColumn->setDouble(i,value);
//    }
//  }
//  catch(std::exception& ex)
//  {
//    throw std::runtime_error(std::string("fillColumn failed: ") + ex.what() );
//  }
//}

bool TableWorkspace::hasColumn(const std::string& colName) const
{
    column_const_it ci = std::find_if(m_columns.begin(),m_columns.end(),FindName(colName));
    return ci != m_columns.end();
}

/**
 * Add a column of type "double" and optionally set its plot role.
 * @param name :: Column name
 * @param plotRole :: NumericColumn::PlotRole: one of X, Y, Z, xError, yError. Default is Unset.
 */
Column_ptr TableWorkspace::addDoubleColumn(const std::string& name, int plotRole)
{
  auto column = addColumn("double",name);
  auto xColumn = static_cast<API::TableColumn<double>*>(column.get());
  xColumn->asNumeric()->setPlotRole(static_cast<API::NumericColumn::PlotRole>(plotRole));
  return column;
}

/**
 * Return reference to the data of a "double" column
 */
std::vector<double>& TableWorkspace::getDoubleData(const std::string& name)
{
  auto xColumn = static_cast<API::TableColumn<double>*>(getColumn(name).get());
  if ( !xColumn ) throw std::runtime_error("Column " + name + " is not double");
  return xColumn->data();
}

/**
 * Set the data in a "double" column.
 * @param name :: The column name to get the data. Must exist and be double type.
 * @param data :: Pointer to the data.
 * @param n :: Size of the data. Must be equal to the row count.
 */
void TableWorkspace::setDoubleData(const std::string& name, const double* data, size_t n)
{
  auto xColumn = static_cast<API::TableColumn<double>*>(getColumn(name).get());
  if ( !xColumn ) throw std::runtime_error("Column " + name + " is not double");
  if ( n != rowCount() ) throw std::runtime_error("Data size isn't equal to the row count.");
  xColumn->data().assign(data,data+n);
}

/**
 * Set the data in a "double" column.
 * @param name :: The column name to get the data. Must exist and be double type.
 * @param data :: Vector containing the data. The size must be equal to the row count.
 */
void TableWorkspace::setDoubleData(const std::string& name, const std::vector<double>& data)
{
  setDoubleData( name, data.data(), data.size() );
}

} // API
