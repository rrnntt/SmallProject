#ifndef TEACHQT_DLLEXPORT_H
#define TEACHQT_DLLEXPORT_H

#if defined(IN_TEACHQT)
#  define TEACHQT_EXPORT __declspec( dllexport )
#else
#  define TEACHQT_EXPORT __declspec( dllimport )
#endif

#ifdef _WIN32
#pragma warning( disable: 4251 )
#endif

#endif // TEACHQT_DLLEXPORT_H
