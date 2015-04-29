#ifndef MIBVIDEOSTRATEGY_GLOBAL_H
#define MIBVIDEOSTRATEGY_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MIBVIDEOSTRATEGY_LIBRARY)
#  define MIBVIDEOSTRATEGYSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MIBVIDEOSTRATEGYSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace mibot
{

class VideoStreamFrameData
{
public:
    char * ptr;
    unsigned int size;
};

}

#endif // MIBVIDEOSTRATEGY_GLOBAL_H
