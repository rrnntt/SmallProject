#ifndef DATAOBJECTS_DLLEXPORT_H
#define DATAOBJECTS_DLLEXPORT_H

#include "Kernel/System.h"

#if defined(IN_DATAOBJECTS)
#  define DATAOBJECTS_EXPORT DLLEXPORT
#else
#  define DATAOBJECTS_EXPORT DLLIMPORT
#endif

#ifdef _WIN32
#pragma warning( disable: 4251 )
#endif

#endif // DATAOBJECTS_DLLEXPORT_H
