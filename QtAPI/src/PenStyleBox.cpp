#include "QtAPI/PenStyleBox.h"

#include <algorithm>

namespace QtAPI
{

const Qt::PenStyle PenStyleBox::patterns[] = {
    Qt::SolidLine,
    Qt::DashLine,
    Qt::DotLine,
    Qt::DashDotLine,
    Qt::DashDotDotLine
};

PenStyleBox::PenStyleBox(QWidget *parent) : QComboBox(parent)
{
  setEditable(false);
  addItem("_____");
  addItem("_ _ _");
  addItem(".....");
  addItem("_._._");
  addItem("_.._..");
}

void PenStyleBox::setStyle(const Qt::PenStyle& style)
{
  // Avoid compiler warnings relating to patterns + sizeof(patterns) being out of range
  size_t n = sizeof(patterns);
  for(size_t i = 0; i < n ; ++i)
  {
    if (patterns[i] == style)
    {
      setCurrentIndex(int(i));
      return;
    }
  }
  setCurrentIndex(0); // default style is solid.
//  const Qt::PenStyle*ite = std::find(patterns, patterns + sizeof(patterns), style);
//  if (ite == patterns + sizeof(patterns))
//    this->setCurrentIndex(0); // default style is solid.
//  else
//    this->setCurrentIndex(int(ite - patterns));
}

Qt::PenStyle PenStyleBox::penStyle(int index)
{
  if (index < (int)sizeof(patterns))
    return patterns[index];
  else
    return Qt::SolidLine; // default style is solid. 
}

Qt::PenStyle PenStyleBox::style() const
{
  size_t i = this->currentIndex();
  if (i < sizeof(patterns))
    return patterns[i];
  else
    return Qt::SolidLine; // default style is solid. 
}

int PenStyleBox::styleIndex(const Qt::PenStyle& style)
{
  // Avoid compiler warnings relating to patterns + sizeof(patterns) being out of range
  size_t n = sizeof(patterns);
  for(size_t i = 0; i < n ; ++i)
  {
    if (patterns[i] == style) return int(i);
  }
  return 0; // default style is solid.
//  const Qt::PenStyle*ite = std::find(patterns, patterns + sizeof(patterns), style);
//  if (ite == patterns + sizeof(patterns))
//    return 0; // default style is solid.
//  else
//    return (int(ite - patterns));
}

} // QtAPI
