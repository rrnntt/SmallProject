#ifndef NUMERIC_DLLEXPORT_H
#define NUMERIC_DLLEXPORT_H

#ifdef IN_NUMERIC
#define NUMERIC_DLL __declspec( dllexport )
#else
#define NUMERIC_DLL __declspec( dllimport )
#endif

#endif /*NUMERIC_DLLEXPORT_H*/
