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

typedef Result<bool, QVariant> BoolVariantResult_t;

/**
 * @brief UuidType_t
 */
typedef QString UuidType_t;

/**
 *  Stores collection of UUID's for mass operation.
 *  Earlier it was <qint32>, now QString. Who knows what will else?
 * @brief UuidVector_t
 */
typedef QVector<UuidType_t> UuidVector_t;

//typedef Result<qint32, QString> Int32Result_t;

template <qint32 v>
struct Int2Type {
    enum { value = v };
};

typedef Int2Type<1> V1_t;
typedef Int2Type<2> V2_t;
typedef Int2Type<3> V3_t;
typedef Int2Type<4> V4_t;
typedef Int2Type<5> V5_t;


}

#endif // STORAGE_GLOBALS

