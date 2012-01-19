#ifndef QtAPI_TABLEDIALOG_H
#define QtAPI_TABLEDIALOG_H

#include "QtAPI/DllExport.h"
#include <boost/shared_ptr.hpp>
#include <QDialog>

namespace Ui {
    class TableDialog;
}

namespace DataObjects
{
  class TableWorkspace;
}

namespace QtAPI
{

class QTAPI_EXPORT TableDialog: public QDialog {
    Q_OBJECT
public:
  TableDialog(QWidget *parent,boost::shared_ptr<DataObjects::TableWorkspace> ws);
    ~TableDialog();

protected:
    void changeEvent(QEvent *e);
    void init();
    void initFormulaPage();
    void initDistributionPage();
    int selectedColumn();
    void accept();
    void apply();
    void applyDistribution();

protected slots:
    void updateDistributionPage(int);

private:
  Ui::TableDialog *ui;
  boost::shared_ptr<DataObjects::TableWorkspace> m_workspace;
};

} // QtAPI

#endif // QtAPI_TABLEDIALOG_H
