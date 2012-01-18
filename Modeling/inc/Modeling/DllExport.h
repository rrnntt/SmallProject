#ifndef MODELING_GLOBAL_H
#define MODELING_GLOBAL_H

#if defined(IN_MODELING)
#  define MODELING_EXPORT __declspec( dllexport )
#else
#  define MODELING_EXPORT __declspec( dllimport )
#endif

#ifdef _WIN32
#pragma warning( disable: 4251 )
#endif

#endif // MODELING_GLOBAL_H
