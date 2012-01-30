#ifndef QtAPI_TABLEDIALOG_H
#define QtAPI_TABLEDIALOG_H

#include "QtAPI/DllExport.h"
#include <boost/shared_ptr.hpp>
#include <QDialog>

class QAbstractButton;

namespace Ui {
    class TableDialog;
}

namespace DataObjects
{
  class TableWorkspace;
}

namespace Formula
{
  class Namespace;
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
    void initColumns();
    void initFormulaPage();
    void initDistributionPage();
    int selectedColumn();
    void accept();
    void apply();
    void applyDistribution();
    void applyFormula();

protected slots:
    void updateDistributionPage(int);
    void applyRowCount();
    void applyButton(QAbstractButton*);
    void addColumn();
    void removeColumn();

private:
  Ui::TableDialog *ui;
  boost::shared_ptr<DataObjects::TableWorkspace> m_workspace;
  boost::shared_ptr<Formula::Namespace> m_namespace;
};

} // QtAPI

#endif // QtAPI_TABLEDIALOG_H
