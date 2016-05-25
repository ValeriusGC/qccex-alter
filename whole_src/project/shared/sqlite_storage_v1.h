#ifndef SQLITE_STORAGE_V1
#define SQLITE_STORAGE_V1

#include <QString>

namespace storage {

namespace sqlite {

namespace v1 {

extern const qint32 VERSION;

struct TableCfg {
    static const QString TBL_NAME;
    static const QString FLD_ID;
    static const QString FLD_THE_KEY;
    static const QString FLD_THE_VALUE;

    static const QString QRY_CREATE;
};

struct TableNote {
    static const QString TBL_NAME;
    static const QString FLD_ID;
    static const QString FLD_THE_TEXT;

    static const QString QRY_CREATE;
};


} // namespace v1

} // namespace sqlite

} // namespace storage

#endif // SQLITE_STORAGE_V1

