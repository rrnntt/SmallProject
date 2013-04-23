#include "QtAPI/PlotObject.h"

namespace QtAPI
{

/**
 * Constructor.
 */
PlotObject::PlotObject()
{
}

PlotObject::PlotObject(const PlotObject& right):
m_title(right.m_title),
m_pen(right.m_pen)
{
}

} // QtAPI
