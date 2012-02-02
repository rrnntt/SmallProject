#ifndef NUMERIC_DLLEXPORT_H
#define NUMERIC_DLLEXPORT_H

#pragma warning( disable: 4251 )

#ifdef IN_NUMERIC
#define NUMERIC_EXPORT __declspec( dllexport )
#else
#define NUMERIC_EXPORT __declspec( dllimport )
#endif

#define UNUSED_ARG(x) (void)x

#endif /*NUMERIC_DLLEXPORT_H*/
