#ifndef MIBACCESSGLOBALS_H
#define MIBACCESSGLOBALS_H

#include <QtCore/qglobal.h>

#if defined(MIBACCESS_LIBRARY)
#  define MIBACCESSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MIBACCESSSHARED_EXPORT Q_DECL_IMPORT
#endif


#endif // MIBACCESSGLOBALS_H
