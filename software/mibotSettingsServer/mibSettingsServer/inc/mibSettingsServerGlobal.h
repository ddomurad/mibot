#ifndef MIBSETTINGSSERVER_GLOBAL_H
#define MIBSETTINGSSERVER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MIBSETTINGSSERVER_LIBRARY)
#  define MIBSETTINGSSERVERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MIBSETTINGSSERVERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MIBSETTINGSSERVER_GLOBAL_H
