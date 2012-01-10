#ifndef NUMERIC_DLLEXPORT_H
#define NUMERIC_DLLEXPORT_H

#ifdef IN_NUMERIC
#define NUMERIC_EXPORT __declspec( dllexport )
#else
#define NUMERIC_EXPORT __declspec( dllimport )
#endif

#endif /*NUMERIC_DLLEXPORT_H*/
