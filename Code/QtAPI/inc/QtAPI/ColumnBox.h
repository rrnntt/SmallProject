#ifndef QTAPI_COLUMNBOX_H
#define QTAPI_COLUMNBOX_H

#include "QtAPI/DllExport.h"

#include <boost/shared_ptr.hpp>
#include <QComboBox>

namespace API
{
  class TableWorkspace;
}

namespace QtAPI
{

/**

A modified combo-box used for column slection.

  */
class QTAPI_EXPORT ColumnBox: public QComboBox
{
    Q_OBJECT

public:
  ColumnBox(QWidget *parent = 0);
  void setTableWorkspace(boost::shared_ptr<API::TableWorkspace> ws, bool numeric = false, int prefPlotRole = 0);
private:
  boost::shared_ptr<API::TableWorkspace> m_workspace;
};

} // QtAPI

#endif // QTAPI_COLUMNBOX_H
