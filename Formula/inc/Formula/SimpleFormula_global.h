#ifndef SIMPLEFORMULA_GLOBAL_H
#define SIMPLEFORMULA_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SIMPLEFORMULA_LIBRARY)
#  define SIMPLEFORMULASHARED_EXPORT Q_DECL_EXPORT
#else
#  define SIMPLEFORMULASHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SIMPLEFORMULA_GLOBAL_H