#ifndef KERNEL_GLOBAL_H
#define KERNEL_GLOBAL_H

#if defined(IN_KERNEL)
#  define KERNEL_EXPORT __declspec( dllexport )
#else
#  define KERNEL_EXPORT __declspec( dllimport )
#endif

#ifdef _WIN32
#pragma warning( disable: 4251 )
#endif

#endif // KERNEL_GLOBAL_H
