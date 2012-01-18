#ifndef QTAPI_GLOBAL_H
#define QTAPI_GLOBAL_H

#if defined(IN_QTAPI)
#  define QTAPI_EXPORT __declspec( dllexport )
#else
#  define QTAPI_EXPORT __declspec( dllimport )
#endif

#ifdef _WIN32
#pragma warning( disable: 4251 )
#endif

#endif // QTAPI_GLOBAL_H
