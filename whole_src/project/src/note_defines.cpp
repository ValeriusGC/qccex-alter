#include <QHash>

#include "note_defines.h"

#ifdef QT_DEBUG

QHash<QString, QString> &g_objMap() {
    static QHash<QString, QString> objMap;
    return objMap;
}

#endif
