#ifndef API_COLUMNVECTOR_H
#define API_COLUMNVECTOR_H

#include "API/DllExport.h"
#include "API/TableColumn.h"

#include <vector>

namespace API
{

template<typename T>
class ColumnVector
{
public:
  ColumnVector():
  m_data(nullptr)
  {
  }
  ColumnVector(Column_ptr column):
  m_data(nullptr)
  {
    *this = column;
  }
  ColumnVector& operator=(Column_ptr column)
  {
    TableColumn<T>* tc = dynamic_cast< TableColumn<T>* >(column.get());
    if (tc)
    {
      m_column = column;
      m_data = &tc->m_data;
    }
    return *this;
  }
  bool isValid() const {return m_data != nullptr;}
  operator bool() const {return isValid();}
  const T& operator[](size_t i) const {return m_data->at(i);}
  T& operator[](size_t i) {return m_data->at(i);}
  std::vector<T> * getStdVector() {return m_data;}
protected:
  std::vector<T> *m_data;
  Column_ptr m_column;
};

} // API
#endif // API_COLUMNVECTOR_H
