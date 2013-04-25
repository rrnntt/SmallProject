#ifndef QtAPI_TABLEDIALOG_H
#define QtAPI_TABLEDIALOG_H

#include "QtAPI/DllExport.h"
#include <boost/shared_ptr.hpp>
#include <QDialog>

class QAbstractButton;

namespace Ui {
    class TableDialog;
}

namespace API
{
  class TableWorkspace;
}

namespace QtAPI
{

class QTAPI_EXPORT TableDialog: public QDialog {
    Q_OBJECT
public:
  TableDialog(QWidget *parent,boost::shared_ptr<API::TableWorkspace> ws);
    ~TableDialog();

protected:
    void changeEvent(QEvent *e);
    void init();
    void initColumns();
    void initFormulaPage();
    void initDistributionPage();
    void initFunctionPage();
    int selectedColumn();
    void accept();
    void apply();
    void applyDistribution();
    void applyFormula();
    void applyFunction();

protected slots:
    void updateDistributionPage(int);
    void applyRowCount();
    void applyButton(QAbstractButton*);
    void addColumn();
    void removeColumn();
    void formulaChanged();

private:
  Ui::TableDialog *ui;
  boost::shared_ptr<API::TableWorkspace> m_workspace;
  /// tracks status of changes to make sure they are not applied twice:
  /// this can be undesirable when formulas use old values to calculate new ones
  bool m_formulaChangesSaved;
};

} // QtAPI

#endif // QtAPI_TABLEDIALOG_H
