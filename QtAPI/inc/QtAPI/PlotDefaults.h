#ifndef QTAPI_PLOTDEFAULTS_H
#define QTAPI_PLOTDEFAULTS_H

#include "DllExport.h"
#include <QFont>
#include <QColor>
#include <QList>

namespace QtAPI
{

class QTAPI_EXPORT PlotDefaults
{
public:
  static PlotDefaults& instance();
  void save();

  QFont axisFont()const{return m_axis_font;}
  void setAxisFont(const QFont& font){m_axis_font = font;}

  QFont titleFont()const{return m_title_font;}
  void setTitleFont(const QFont& font){m_title_font = font;}

  QColor titleColor()const{return m_title_color;}
  void setTitleColor(const QColor& color){m_title_color = color;}

  QColor getCurveColor(int i)const;
private:
    PlotDefaults();
    QFont m_axis_font;
    QFont m_title_font;
    QColor m_title_color;
    QList<QColor> m_curve_colors;
};

} // QtAPI

#endif // QTAPI_PLOTDEFAULTS_H
