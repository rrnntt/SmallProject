#ifndef FORMULA_DLLEXPORT_H
#define FORMULA_DLLEXPORT_H

#if defined(IN_FORMULA)
#  define FORMULA_EXPORT __declspec( dllexport )
#else
#  define FORMULA_EXPORT __declspec( dllimport )
#endif

#ifdef _WIN32
#pragma warning( disable: 4251 )
#endif

#endif // FORMULA_DLLEXPORT_H
