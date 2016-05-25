#ifndef STORAGE_GLOBALS
#define STORAGE_GLOBALS

#include <QtGlobal>

#include "shared_result.h"

namespace storage {

//extern const qint32 V1;
//extern const qint32 V2;
//extern const qint32 V3;

#define STORAGE_NO_EXISTS       -1
#define STORAGE_V1              1
#define STORAGE_V2              2
#define STORAGE_V3              3

extern const qint32 STORAGE_CURR_VER;

typedef Result<qint32, QVariant> BoolVariantResult_t;
//typedef Result<qint32, QString> Int32Result_t;

}

#endif // STORAGE_GLOBALS

