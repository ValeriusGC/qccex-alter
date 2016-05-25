#include <QHash>

#include "shared_def.h"

namespace vfx_shared {

#ifdef QT_DEBUG

QHash<QString, QString> &g_objMap() {
    static QHash<QString, QString> objMap;
    return objMap;

}

}

#endif
