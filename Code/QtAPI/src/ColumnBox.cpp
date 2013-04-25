#include "QtAPI/ColumnBox.h"
#include "API/TableWorkspace.h"
#include "API/NumericColumn.h"

namespace QtAPI
{

/**
  Constructor.
  @param parent :: A parent widget.
 */
ColumnBox::ColumnBox(QWidget *parent):QComboBox(parent)
{
}

/**
  Set table workspace which column will be selected.
  @param ws :: Table workspace to set.
  @param numeric :: True to select only numeric columns.
  @param prefPlotRole :: Prefered plot role. Actual type id API::NumericColumn::PlotRole.
     If set to other than API::NumericColumn::Unset make current the first found column
     with this role.
  */
void ColumnBox::setTableWorkspace(boost::shared_ptr<API::TableWorkspace> tws, bool numeric, int prefPlotRole)
{
    this->clear();
    m_workspace = tws;
    if ( !tws ) return;
    QStringList colNames;
    QString currentColumn;
    API::NumericColumn::PlotRole role = static_cast<API::NumericColumn::PlotRole>(prefPlotRole);
    for(size_t i = 0; i < tws->columnCount(); ++i)
    {
        auto col = tws->getColumn(i);
        QString qName = QString::fromStdString(col->name());
        if ( numeric )
        {
            // select only numeric columns
            auto num = col->asNumeric();
            colNames << qName;
            if ( role != API::NumericColumn::Unset &&
                 currentColumn.isEmpty() &&
                 num->getPlotRole() == role
               )
            {
                currentColumn = qName;
            }
        }
        else
        {
            // add all columns
            colNames << qName;
        }
    }
    insertItems(0,colNames);
    if ( !currentColumn.isEmpty() )
    {
        setCurrentIndex(findText(currentColumn));
    }
}

} // QtAPI
