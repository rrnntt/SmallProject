#ifndef DATAOBJECTS_DLLEXPORT_H
#define DATAOBJECTS_DLLEXPORT_H

#if defined(IN_DATAOBJECTS)
#  define DATAOBJECTS_EXPORT __declspec( dllexport )
#else
#  define DATAOBJECTS_EXPORT __declspec( dllimport )
#endif

#ifdef _WIN32
#pragma warning( disable: 4251 )
#endif

#endif // DATAOBJECTS_DLLEXPORT_H
