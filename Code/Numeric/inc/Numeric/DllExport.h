#ifndef NUMERIC_DLLEXPORT_H
#define NUMERIC_DLLEXPORT_H

#pragma warning( disable: 4251 )
#pragma warning( disable: 4250 )

#include "Kernel/System.h"

#ifdef IN_NUMERIC
#define NUMERIC_EXPORT DLLEXPORT
#else
#define NUMERIC_EXPORT DLLIMPORT
#endif

#define UNUSED_ARG(x) (void)x

#endif /*NUMERIC_DLLEXPORT_H*/
