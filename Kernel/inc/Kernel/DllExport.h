#ifndef KERNEL_GLOBAL_H
#define KERNEL_GLOBAL_H

#include "Kernel/System.h"

#if defined(IN_KERNEL)
#  define KERNEL_EXPORT DLLEXPORT
#else
#  define KERNEL_EXPORT DLLIMPORT
#endif

#ifdef _WIN32
#pragma warning( disable: 4251 )
#endif

#endif // KERNEL_GLOBAL_H
