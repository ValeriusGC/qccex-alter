#ifndef SQLITE_STORAGE_V3_H
#define SQLITE_STORAGE_V3_H

#include <QString>
#include <QSqlDatabase>
#include "shared_result.h"

namespace storage {

namespace sqlite {

namespace v3 {

extern const qint32 VERSION;

//  1. Changed tables 'tbl_config', 'tbl_author', 'tbl_note'

struct TableCfg {
    //  1. Added fields 'ts_create' : creation timestamp
    //  2. Added fields 'ts_edit' : last edit timestamp
    static const QString TBL_NAME;
    static const QString FLD_ID;
    static const QString FLD_KEY;
    static const QString FLD_VALUE;
    static const QString FLD_DESCR;
    static const QString FLD_TS_CREATE;
    static const QString FLD_TS_EDIT;

    static const QString QRY_CREATE;
};

struct TableNote {
    //  Added fields
    //  'ts_create' : creation timestamp
    //  'ts_edit' : last edit timestamp
    //  'del' : marker for deleted record

    static const QString TBL_NAME;
    static const QString FLD_ID;
    static const QString FLD_TEXT;
    static const QString FLD_FK_AUTHOR;
    static const QString FLD_TS_CREATE;
    static const QString FLD_TS_EDIT;
    static const QString FLD_DEL;  ///< Marks record as deleted. INTEGER

    static const QString QRY_CREATE;
};

struct TableAuthor {
    //  Added fields
    //  1. 'ts_create' : creation timestamp
    //  2. 'ts_edit' : last edit timestamp
    //  3. 'del' : marker for deleted record

    static const QString TBL_NAME;
    static const QString FLD_ID;
    static const QString FLD_TITLE;
    static const QString FLD_TS_CREATE;
    static const QString FLD_TS_EDIT;
    /**
     * Marks record as deleted. INTEGER
     * @brief FLD_DEL
     */
    static const QString FLD_DEL;

    static const QString QRY_CREATE;

    static BoolResult_t create(QSqlDatabase db);
};

struct TableTags {
    static const QString TBL_NAME;

    static const QString FLD_ID;
    static const QString FLD_TEXT;
    static const QString FLD_DEL; ///< Marks record as deleted. INTEGER

    static BoolResult_t create(QSqlDatabase db);
};

struct TableTagsNotes {
    static const QString TBL_NAME;

    static const QString FLD_ID;
    static const QString FLD_FK_TAGS;
    static const QString FLD_FK_NOTES;

    static BoolResult_t create(QSqlDatabase db);
};

} // namespace v3

} // namespace sqlite

} // namespace storage


#endif // SQLITE_STORAGE_V3_H
