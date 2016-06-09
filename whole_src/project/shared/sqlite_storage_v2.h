#ifndef SQLITE_STORAGE_V2_H
#define SQLITE_STORAGE_V2_H

#include <QString>
#include <QSqlDatabase>
#include "shared_result.h"
#include "sqlstorageelement.h"

namespace storage {

namespace sqlite {

namespace v2 {

extern const qint32 VERSION;

//  1. Added table 'tbl_author'
//  2. Changed tables 'tbl_config', 'tbl_note'

struct TableCfg {
    //  1. Added field 'descr' for description
    static const QString TBL_NAME;
    static const QString FLD_ID;
    static const QString FLD_KEY;
    static const QString FLD_VALUE;
    static const QString FLD_DESCR;

    static const QString QRY_CREATE;    
};

struct TableNote {
    //  1. Added field 'descr': foreign key for table 'author'
    static const QString TBL_NAME;
    static const QString FLD_ID;
    static const QString FLD_TEXT;
    static const QString FLD_FK_AUTHOR;

    static const QString QRY_CREATE;
};

struct TableAuthor : public StorageElement {

    static const QString TBL_NAME;
    static const QString FLD_ID;
    static const QString FLD_TITLE;

//    static const QString QRY_CREATE;
//    static BoolResult_t create(QSqlDatabase db);
protected:
    // StorageElement interface
    virtual BoolResult_t doCreate(QSqlDatabase db);
    virtual BoolResult_t doUpgradeFromV1(QSqlDatabase db);
    virtual BoolResult_t doUpgradeFromV2(QSqlDatabase db);

};


} // namespace v2

} // namespace sqlite

} // namespace storage


#endif // SQLITE_STORAGE_V2_H
