#ifndef API_DLLEXPORT_H
#define API_DLLEXPORT_H

#if defined(IN_API)
#  define API_EXPORT __declspec( dllexport )
#else
#  define API_EXPORT __declspec( dllimport )
#endif

#ifdef _WIN32
#pragma warning( disable: 4251 )
#endif

#endif // API_DLLEXPORT_H
