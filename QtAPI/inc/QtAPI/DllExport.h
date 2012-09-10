#ifndef QTAPI_GLOBAL_H
#define QTAPI_GLOBAL_H

#include "Kernel/System.h"

#if defined(IN_QTAPI)
#  define QTAPI_EXPORT DLLEXPORT
#else
#  define QTAPI_EXPORT DLLIMPORT
#endif

#ifdef _WIN32
#pragma warning( disable: 4251 )
#endif

#endif // QTAPI_GLOBAL_H
