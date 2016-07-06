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

/**
 * @brief The Const class
 *  Storage constants
 */
class Const {
public:
    static const qint32 SUCCESS; ///< Result is OK
    static const qint32 INVALID; ///< Result is invalid

    static const qint32 E_RES_NO_RESULT; ///< No result code in response
    static const qint32 E_REQ_WRONG_TYPE; ///< Request has wrong type. hdr sent back in [body][data]
    static const qint32 E_BAD_ALGORITHM; ///< Bad algorithm in function (maybe in if/else or some…)

    static const qint32 REQ_TYPE_GET_NOTES; ///< Get notes
    static const qint32 REQ_TYPE_DEL_NOTES; ///< Delete notes
    static const qint32 REQ_TYPE_ADD_NOTES; ///< Add notes
    static const qint32 REQ_TYPE_COUNT_NOTES; ///< Get count of notes

    static const QString FLD_UUID; //Standard UUID-field for Note-object, etc
    static const QString FLD_DEL; //Standard DELETED-field for Note-object, etc
    static const QString FLD_AUTHOR_REF; //AUTHOR-reference for Note-object (ref. to Author-object)
    static const QString FLD_TS_CREATE; //Standard creation timestamp for Note-object (int64)
    static const QString FLD_TS_EDIT; //Standard last edit timestamp for Note-object (int64)
    static const QString FLD_THE_TEXT; //Standard text for Note-object

    static const QString BODY_SET_NOTES; //Set of notes in 'body'
    static const QString BODY_TS_EDIT_BOTTOM; //Low border for requesting records
    static const QString BODY_TS_EDIT_TOP; //High border for requesting records

};

}

#endif // STORAGE_GLOBALS

