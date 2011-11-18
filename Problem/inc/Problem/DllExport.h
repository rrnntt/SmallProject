#ifndef PROBLEM_DLLEXPORT_H
#define PROBLEM_DLLEXPORT_H

#if defined(IN_PROBLEM)
#  define PROBLEM_EXPORT __declspec( dllexport )
#else
#  define PROBLEM_EXPORT __declspec( dllimport )
#endif

#ifdef _WIN32
#pragma warning( disable: 4251 )
#endif

#endif // PROBLEM_DLLEXPORT_H