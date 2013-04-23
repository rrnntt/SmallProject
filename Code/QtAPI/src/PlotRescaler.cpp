#include "QtAPI/PlotRescaler.h"

namespace QtAPI
{

  void ConstSizeRescaler::rescale(const QSize& oldSize, const QSize& newSize) const
  {
    double width = m_plot->getXEnd() - m_plot->getXStart();
    if (width == 0) return;
    width *= double(newSize.width()) / oldSize.width() / 2;
    double height = m_plot->getYEnd() - m_plot->getYStart();
    if (height == 0) return;
    height *= double(newSize.height()) / oldSize.height() / 2;

    double x = (m_plot->getXStart() + m_plot->getXEnd()) / 2;
    double y = (m_plot->getXStart() + m_plot->getXEnd()) / 2;

    double x1 = x - width;
    double x2 = x + width;
    double y1 = y - height;
    double y2 = y + height;

    m_plot->setAxisScale(Plot::xBottom,x1,x2);
    m_plot->setAxisScale(Plot::xTop,x1,x2);
    m_plot->setAxisScale(Plot::yLeft,y1,y2);
    m_plot->setAxisScale(Plot::yRight,y1,y2);

  }

} // namespace QtAPI
