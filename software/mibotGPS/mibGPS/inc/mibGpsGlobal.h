#ifndef MIBGPS_GLOBAL_H
#define MIBGPS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MIBGPS_LIBRARY)
#  define MIBGPSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MIBGPSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MIBGPS_GLOBAL_H
