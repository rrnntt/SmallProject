#ifndef GOBLIN_COMDIFFDIALOG_H
#define GOBLIN_COMDIFFDIALOG_H

#include "Goblin/DllExport.h"
#include "Goblin/comdiff.h"

#include <QtGui/QDialog>

namespace Ui {
  class ComDiffDialog;
}

namespace Goblin
{

class GOBLIN_EXPORT ComDiffDialog: public QDialog
{
  Q_OBJECT
public:
  ComDiffDialog(QWidget *parent);
  ~ComDiffDialog(){}
private slots:
  void loadLineList();
  void loadEnergyList();
  void createEnergyList();
  void setLists();
  void find();
private:
  void updateLineListCB();
  void updateEnergyListCBs();
  Ui::ComDiffDialog *m_form;
  comdiff m_cd;
};

} // namespace Goblin
#endif // GOBLIN_COMDIFFDIALOG_H
