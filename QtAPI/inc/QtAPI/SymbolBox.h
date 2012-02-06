#ifndef SYMBOLBOX_H
#define SYMBOLBOX_H

#include <QComboBox>
#include <qwt_symbol.h>

namespace QtAPI
{

//! A modified QComboBox allowing to choose a QwtSmbol style.
/**
 * This is a simple hack on top of the QComboBox class.
 \image html images/symbol_box.png
 */
class SymbolBox : public QComboBox
{
  Q_OBJECT
public:
  //! Constructor.
  /**
   * \param parent parent widget (only affects placement of the widget)
   */
  SymbolBox(bool showNoSymbol = true, QWidget *parent = 0);

  void setStyle(const QwtSymbol::Style& c);
  QwtSymbol::Style selectedSymbol() const;

  static QwtSymbol::Style style(int index);
  static int symbolIndex(const QwtSymbol::Style& style);
  static QList<int> defaultSymbols();

signals:
  //! Signal emitted when the box gains focus
  void activated(SymbolBox *);

protected:
  void init(bool showNoSymbol);
  void focusInEvent(QFocusEvent *);

private:
  static const QwtSymbol::Style symbols[];
};

} // QtAPI

#endif
