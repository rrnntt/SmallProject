#include "AddTableColumnDialog.h"
#include "ui_AddTableColumnDialog.h"
#include "Table.h"

#include "DataObjects/ColumnFactory.h"

#include <iostream>
#include <stdexcept>

AddTableColumnDialog::AddTableColumnDialog(Table* table) :
    QDialog(table),
    ui(new Ui::AddTableColumnDialog),
    m_table(table)
{
  ui->setupUi(this);
  init();
}

AddTableColumnDialog::~AddTableColumnDialog()
{
    delete ui;
}

void AddTableColumnDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void AddTableColumnDialog::init()
{
  std::vector<std::string> keys = DataObjects::ColumnFactory::instance().keys();
  QStringList columnTypes;
  std::for_each(keys.begin(),keys.end(),[&columnTypes](std::string& key){
    columnTypes << QString::fromStdString(key);
  });
  ui->cbColumnType->insertItems(0,columnTypes);
  ui->leColumnName->setText("Name");
  ui->leColumnName->setSelection(0,4);
}

//void AddTableColumnDialog::accept()
//{
//  apply();
//  close();
//}
//
//void AddTableColumnDialog::apply()
//{
//}

std::string AddTableColumnDialog::getType() const
{
  return ui->cbColumnType->currentText().toStdString();
}

std::string AddTableColumnDialog::getName() const
{
  return ui->leColumnName->text().toStdString();
}
