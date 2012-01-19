#ifndef GOBLIN_DLLEXPORT_H
#define GOBLIN_DLLEXPORT_H

#ifdef IN_GOBLIN
#define GOBLIN_EXPORT __declspec( dllexport )
#else
#define GOBLIN_EXPORT __declspec( dllimport )
#endif

#ifdef _WIN32
#pragma warning( disable: 4251 )
#endif

#endif /*GOBLIN_DLLEXPORT_H*/
