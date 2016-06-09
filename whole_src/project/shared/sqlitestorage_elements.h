#ifndef SQLITESTORAGE_ELEMENTS_H
#define SQLITESTORAGE_ELEMENTS_H

#include <QString>
#include <QSqlDatabase>

#include "shared_result.h"
#include "sqlstorageelement.h"

namespace storage {

namespace sqlite {

//  1. Changed tables 'tbl_config', 'tbl_author', 'tbl_note'

struct TableCfg : public IStorageOperations {

    TableCfg();
    ~TableCfg();
    //  1. Added fields 'ts_create' : creation timestamp
    //  2. Added fields 'ts_edit' : last edit timestamp
    static const QString TBL_NAME;
    static const QString FLD_ID;
    static const QString FLD_KEY;
    static const QString FLD_VALUE;
    static const QString FLD_DESCR;
    static const QString FLD_TS_CREATE;
    static const QString FLD_TS_EDIT;

    protected:
        // StorageElement interface
        virtual BoolVariantResult_t doCreate(QSqlDatabase db);
        virtual BoolVariantResult_t doUpgrade(QSqlDatabase db, const V1_t &fromVersion);
        virtual BoolVariantResult_t doUpgrade(QSqlDatabase db, const V2_t &fromVersion);
//        virtual BoolResult_t doUpgradeFromV1(QSqlDatabase db);
//        virtual BoolResult_t doUpgradeFromV2(QSqlDatabase db);
};

struct TableNote : public IStorageOperations  {
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

protected:
    // StorageElement interface
    virtual BoolVariantResult_t doCreate(QSqlDatabase db);
    virtual BoolVariantResult_t doUpgrade(QSqlDatabase db, const V1_t &fromVersion);
    virtual BoolVariantResult_t doUpgrade(QSqlDatabase db, const V2_t &fromVersion);
//    virtual BoolResult_t doUpgradeFromV1(QSqlDatabase db);
//    virtual BoolResult_t doUpgradeFromV2(QSqlDatabase db);
};

struct TableAuthor : public IStorageOperations {
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

protected:
    // StorageElement interface
    virtual BoolVariantResult_t doCreate(QSqlDatabase db);
    virtual BoolVariantResult_t doUpgrade(QSqlDatabase db, const V1_t &fromVersion);
    virtual BoolVariantResult_t doUpgrade(QSqlDatabase db, const V2_t &fromVersion);
//    virtual BoolResult_t doUpgradeFromV1(QSqlDatabase db);
//    virtual BoolResult_t doUpgradeFromV2(QSqlDatabase db);
};

struct TableTags : public IStorageOperations {
    static const QString TBL_NAME;

    static const QString FLD_ID;
    static const QString FLD_TEXT;
    static const QString FLD_DEL; ///< Marks record as deleted. INTEGER

protected:
    // StorageElement interface
    virtual BoolVariantResult_t doCreate(QSqlDatabase db);
    virtual BoolVariantResult_t doUpgrade(QSqlDatabase db, const V1_t &fromVersion);
    virtual BoolVariantResult_t doUpgrade(QSqlDatabase db, const V2_t &fromVersion);
//    virtual BoolResult_t doUpgradeFromV1(QSqlDatabase db);
//    virtual BoolResult_t doUpgradeFromV2(QSqlDatabase db);
};

struct TableTagsNotes : public IStorageOperations {
    static const QString TBL_NAME;

    static const QString FLD_ID;
    static const QString FLD_FK_TAGS;
    static const QString FLD_FK_NOTES;

    //    static BoolResult_t create(QSqlDatabase db);
protected:
    // StorageElement interface
    virtual BoolVariantResult_t doCreate(QSqlDatabase db);
    virtual BoolVariantResult_t doUpgrade(QSqlDatabase db, const V1_t &fromVersion);
    virtual BoolVariantResult_t doUpgrade(QSqlDatabase db, const V2_t &fromVersion);
//    virtual BoolResult_t doUpgradeFromV1(QSqlDatabase db);
//    virtual BoolResult_t doUpgradeFromV2(QSqlDatabase db);
};

} // namespace sqlite

} // namespace storage


#endif // SQLITESTORAGE_ELEMENTS_H
