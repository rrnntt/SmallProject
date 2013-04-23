#ifndef GOBLIN_DLLEXPORT_H
#define GOBLIN_DLLEXPORT_H

#include "Kernel/System.h"

#ifdef IN_GOBLIN
#define GOBLIN_EXPORT DLLEXPORT
#else
#define GOBLIN_EXPORT DLLIMPORT
#endif

#ifdef _WIN32
#pragma warning( disable: 4251 )
#endif

#endif /*GOBLIN_DLLEXPORT_H*/
