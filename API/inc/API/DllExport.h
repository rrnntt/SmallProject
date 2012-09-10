#ifndef API_DLLEXPORT_H
#define API_DLLEXPORT_H

#include "Kernel/System.h"

#if defined(IN_API)
#  define API_EXPORT DLLEXPORT
#else
#  define API_EXPORT DLLIMPORT
#endif

#ifdef _WIN32
#pragma warning( disable: 4251 )
#endif

#endif // API_DLLEXPORT_H
