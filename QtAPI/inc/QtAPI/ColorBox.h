#ifndef QTAPI_COLORBOX_H
#define QTAPI_COLORBOX_H

#include <QComboBox>

namespace QtAPI
{

//! A modified QComboBox used for color selection.
/**
 * It contains a list of 24 predefined colors.
 */
class ColorBox : public QComboBox
{
  Q_OBJECT

public:
  //! Constructor
  /**
   * \param parent parent widget
   */
  ColorBox(QWidget *parent = 0);
  //! Set the current color
  void setColor(const QColor& c);
  //! Return the current color
  QColor color() const;
  //! Return the list of colors
  static QList<QColor> colorList();
  //! Returns the color names
  static QStringList colorNames();
  //! Return the index for a given color
  static int colorIndex(const QColor& c);
  //! Return the color at index 'colorIndex'
  static QColor color(int colorIndex);
  //! Return the default color at index 'colorIndex'
  static QColor defaultColor(int colorIndex);
  //! Returns TRUE if the color is included in the color box, otherwise returns FALSE.
  static bool isValidColor(const QColor& color);
  //! Returns the number of predefined colors
  static int numPredefinedColors();
  //! Returns the color name for the predefined colors
  static QStringList defaultColorNames();
  //! Returns the list of predefined colors
  static QList<QColor> defaultColors();

protected:
  //! Internal initialization function
  void init();
  //! The number of predefined colors
  static const int colors_count = 24;
  //! Array containing the 24 predefined colors
  static const QColor colors[];
};

} // QtAPI

#endif // QTAPI_COLORBOX_H
