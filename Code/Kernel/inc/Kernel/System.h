#ifndef KERNEL_SYSTEM_H
#define KERNEL_SYSTEM_H

#ifdef _WIN32
#define DLLEXPORT __declspec( dllexport )
#define DLLIMPORT __declspec( dllimport )
#else
#define DLLEXPORT
#define DLLIMPORT
#endif

#endif
