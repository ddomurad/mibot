#ifndef MIBDRIVESTRATEGY_GLOBAL_H
#define MIBDRIVESTRATEGY_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MIBDRIVESTRATEGY_LIBRARY)
#  define MIBDRIVESTRATEGYSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MIBDRIVESTRATEGYSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MIBDRIVESTRATEGY_GLOBAL_H
