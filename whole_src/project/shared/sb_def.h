#ifndef SB_DEF
#define SB_DEF

#include <QSqlError>

#include "shared_result.h"

typedef vfx_shared::Result<QSqlError, qint32> SqlIntResult_t;
typedef vfx_shared::Result<QSqlError, QString> SqlStringResult_t;
typedef vfx_shared::Result<QSqlError, QVariant> SqlVariantResult_t;

template <qint32 v>
struct Int2Type {
    enum { value = v };
};

typedef Int2Type<1> V1_t;
typedef Int2Type<2> V2_t;
typedef Int2Type<3> V3_t;
typedef Int2Type<4> V4_t;
typedef Int2Type<5> V5_t;



#endif // SB_DEF

