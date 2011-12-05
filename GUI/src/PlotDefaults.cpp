#include "PlotDefaults.h"

#include <QSettings>

PlotDefaults::PlotDefaults():
m_axis_font("Sefir"),
m_title_font("Times New Roman"),
m_title_color(Qt::black)
{
  QSettings settings;
  settings.beginGroup("Plot");
  m_title_color = settings.value("title_color",Qt::black).value<QColor>();

  // first set defalts, will be used in case of settings failure
  m_title_font.setPixelSize(20);
  m_axis_font.setPixelSize(14);
  m_axis_font.setWeight(QFont::Bold);
  // read fonts from settings
  m_title_font = settings.value("title_font",m_title_font).value<QFont>();
  m_axis_font = settings.value("axis_font",m_axis_font).value<QFont>();

  m_curve_colors.append(Qt::black);
  m_curve_colors.append(Qt::red);
  m_curve_colors.append(Qt::green);
  m_curve_colors.append(Qt::blue);
  m_curve_colors.append(Qt::yellow);
  m_curve_colors.append(Qt::magenta);
  m_curve_colors.append(Qt::gray);
  m_curve_colors.append(Qt::cyan);
  m_curve_colors.append(Qt::darkBlue);
  m_curve_colors.append(Qt::darkCyan);
  m_curve_colors.append(Qt::darkGray);
  m_curve_colors.append(Qt::darkGreen);
  m_curve_colors.append(Qt::darkMagenta);
  m_curve_colors.append(Qt::darkRed);
  m_curve_colors.append(Qt::darkYellow);

}

void PlotDefaults::save()
{
  QSettings settings;
  settings.beginGroup("Plot");
  settings.setValue("title_color",m_title_color);
  settings.setValue("title_font",m_title_font);
  settings.setValue("axis_font",m_axis_font);
}

PlotDefaults& PlotDefaults::instance()
{
  static PlotDefaults inst;
  return inst;
}

QColor PlotDefaults::getCurveColor(int i)const
{
  int j = i % m_curve_colors.size();
  return m_curve_colors[j];
}
