#ifndef PEN_STYLE_BOX_H
#define PEN_STYLE_BOX_H

#include <QComboBox>

namespace QtAPI
{

//! A modified QComboBox allowing to choose a Qt::PenStyle.
/**
 * This is a simple hack on top of the QComboBox class.
 */
class PenStyleBox : public QComboBox
{
  Q_OBJECT

public:
  //! Constructor.
  /**
   * \param parent parent widget (only affects placement of the widget)
   */
  PenStyleBox(QWidget *parent = 0);
  void setStyle(const Qt::PenStyle& style);
  Qt::PenStyle style() const;

  static int styleIndex(const Qt::PenStyle& style);
  static Qt::PenStyle penStyle(int index);

private:
  static const Qt::PenStyle patterns[];
};

} // QtAPI

#endif
