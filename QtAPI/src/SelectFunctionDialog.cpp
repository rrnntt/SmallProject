#include "QtAPI/SelectFunctionDialog.h"
#include "ui_SelectFunctionDialog.h"

#include "Numeric/FunctionFactory.h"

namespace QtAPI
{

/**
 * Constructor.
 */
SelectFunctionDialog::SelectFunctionDialog(QWidget *parent):
QDialog(parent),
m_form(new Ui::SelectFunctionDialog())
{
  m_form->setupUi(this);
  auto funNames = Numeric::FunctionFactory::instance().keys();
  for(auto f = funNames.begin(); f != funNames.end(); ++f)
  {
    m_form->cbFunction->addItem(QString::fromStdString(*f));
  }
  updateForm(0);
  connect(m_form->cbFunction,SIGNAL(currentIndexChanged(int)),this,SLOT(updateForm(int)));
}

/**
 * Destructor.
 */
SelectFunctionDialog::~SelectFunctionDialog()
{
}

/**
 * Shows or hides GUI elements depending on the value in function combo-box
 * @param i :: index of the value in function combo-box
 */
void SelectFunctionDialog::updateForm(int i)
{
  QString fun = m_form->cbFunction->currentText();
  if (fun != "UserFunction1D")
  {
    m_form->lblFormula->setVisible(false);
    m_form->leFormula->setVisible(false);
  }
  else
  {
    m_form->lblFormula->setVisible(true);
    m_form->leFormula->setVisible(true);
  }
}

SelectFunctionDialog::Selection SelectFunctionDialog::getSelection()const
{
  Selection selection;
  selection.function = m_form->cbFunction->currentText().toStdString();
  if (m_form->leFormula->isVisible())
  {
    selection.formula = m_form->leFormula->text().toStdString();
  }
  return selection;
}

} // QtAPI
