#ifndef SHARED_CONST
#define SHARED_CONST

#include <QtCore>

namespace model {

struct Constants {
    /**
     * @brief VERSION
     * Current version of model. Used for interoperations between system parts.
     */
    static const qint32 VERSION;
    static const QString DEFAULT_AUTHOR;
    static const QString TAG_INCOMING;
};

}

#endif // SHARED_CONST

