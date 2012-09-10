#ifndef FORMULA_DLLEXPORT_H
#define FORMULA_DLLEXPORT_H

#include "Kernel/System.h"

#if defined(IN_FORMULA)
#  define FORMULA_EXPORT DLLEXPORT
#else
#  define FORMULA_EXPORT DLLIMPORT
#endif

#ifdef _WIN32
#pragma warning( disable: 4251 )
#endif

#endif // FORMULA_DLLEXPORT_H
