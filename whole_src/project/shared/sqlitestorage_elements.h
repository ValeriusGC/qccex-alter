#ifndef SQLITESTORAGE_ELEMENTS_H
#define SQLITESTORAGE_ELEMENTS_H

#include <QString>
#include <QSqlDatabase>

#include "shared_result.h"
#include "sqlstorageelement.h"
#include "sqlengine.h"

namespace storage {

namespace sqlite {

typedef SqlEngineSharedPtr_t  SqlEngine_t;

struct TableCfg : public StorageDDLOperations<SqlEngine_t> {

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
    virtual BoolVariantResult_t doCreate(const SqlEngine_t &engine);
    virtual BoolVariantResult_t doUpgrade(const SqlEngine_t &engine, const V1_t &fromVersion);
    virtual BoolVariantResult_t doUpgrade(const SqlEngine_t &engine, const V2_t &fromVersion);
    virtual BoolVariantResult_t doUpgrade(const SqlEngine_t &engine, const V3_t &fromVersion);
    virtual BoolVariantResult_t doUpgrade(const SqlEngine_t &engine, const V4_t &fromVersion);
    //        virtual BoolResult_t doUpgradeFromV1(QSqlDatabase db);
    //        virtual BoolResult_t doUpgradeFromV2(QSqlDatabase db);
private:
    BoolVariantResult_t updateVersion(SqlEngineSharedPtr_t enginePtr);
};

struct TableNote : public StorageDDLOperations<SqlEngineSharedPtr_t>  {

    //  Added fields
    //  'ts_create' : creation timestamp
    //  'ts_edit' : last edit timestamp
    //  'del' : marker for deleted record

    // V4:
    // Added field FLD_UUID - for UUID (in RFC4122 and packed to string as BASE64)

    static const QString TBL_NAME;
    static const QString FLD_ID;
    static const QString FLD_UUID;
    static const QString FLD_TEXT;
    static const QString FLD_FK_AUTHOR;
    static const QString FLD_TS_CREATE;
    static const QString FLD_TS_EDIT;
    static const QString FLD_DEL;  ///< Marks record as deleted. INTEGER

protected:
    // StorageElement interface
    virtual BoolVariantResult_t doCreate(const SqlEngine_t &engine);
    virtual BoolVariantResult_t doUpgrade(const SqlEngine_t &engine, const V1_t &fromVersion);
    virtual BoolVariantResult_t doUpgrade(const SqlEngine_t &engine, const V2_t &fromVersion);
    virtual BoolVariantResult_t doUpgrade(const SqlEngine_t &engine, const V3_t &fromVersion);
    virtual BoolVariantResult_t doUpgrade(const SqlEngine_t &engine, const V4_t &fromVersion);
private:
    BoolVariantResult_t createUuids(SqlEngineSharedPtr_t enginePtr);
};

struct TableAuthor : public StorageDDLOperations<SqlEngineSharedPtr_t> {

    //  Added fields
    //  1. 'ts_create' : creation timestamp
    //  2. 'ts_edit' : last edit timestamp
    //  3. 'del' : marker for deleted record

    // V4:
    // Added field FLD_UUID - for UUID (in RFC4122 and packed to string as BASE64)
    static const QString TBL_NAME;
    static const QString FLD_ID;
    static const QString FLD_UUID;
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
    virtual BoolVariantResult_t doCreate(const SqlEngine_t &engine);
    virtual BoolVariantResult_t doUpgrade(const SqlEngine_t &engine, const V1_t &fromVersion);
    virtual BoolVariantResult_t doUpgrade(const SqlEngine_t &engine, const V2_t &fromVersion);
    virtual BoolVariantResult_t doUpgrade(const SqlEngine_t &engine, const V3_t &fromVersion);
    virtual BoolVariantResult_t doUpgrade(const SqlEngine_t &engine, const V4_t &fromVersion);

private:
    BoolVariantResult_t createUuids(SqlEngineSharedPtr_t enginePtr);
};

struct TableTags : public StorageDDLOperations<SqlEngineSharedPtr_t> {
    static const QString TBL_NAME;

    static const QString FLD_ID;
    static const QString FLD_TEXT;
    static const QString FLD_DEL; ///< Marks record as deleted. INTEGER

protected:
    // StorageElement interface
    virtual BoolVariantResult_t doCreate(const SqlEngine_t &engine);
    virtual BoolVariantResult_t doUpgrade(const SqlEngine_t &engine, const V1_t &fromVersion);
    virtual BoolVariantResult_t doUpgrade(const SqlEngine_t &engine, const V2_t &fromVersion);
    virtual BoolVariantResult_t doUpgrade(const SqlEngine_t &engine, const V3_t &fromVersion);
    virtual BoolVariantResult_t doUpgrade(const SqlEngine_t &engine, const V4_t &fromVersion);

};

struct TableTagsNotes : public StorageDDLOperations<SqlEngineSharedPtr_t> {
    static const QString TBL_NAME;

    static const QString FLD_ID;
    static const QString FLD_FK_TAGS;
    static const QString FLD_FK_NOTES;

protected:
    // StorageElement interface
    virtual BoolVariantResult_t doCreate(const SqlEngine_t &engine);
    virtual BoolVariantResult_t doUpgrade(const SqlEngine_t &engine, const V1_t &fromVersion);
    virtual BoolVariantResult_t doUpgrade(const SqlEngine_t &engine, const V2_t &fromVersion);
    virtual BoolVariantResult_t doUpgrade(const SqlEngine_t &engine, const V3_t &fromVersion);
    virtual BoolVariantResult_t doUpgrade(const SqlEngine_t &engine, const V4_t &fromVersion);
};

/**
 * @brief The TableTmpFromSrv struct
 *  V5. Stores data of last sync from Server before applying it locally.
 *
 *      16/06/21
 *
 *      Данные о синхронизации
 *      - время: сначала - начала процесса, по окончании - завершения его
 *      - результат, в т.ч. возможен промежуточный
 *      - адрес сервера
 *      - UUID сервера (сервер может менять адрес, но  не UUID)
 *      - Нижний порог времени
 *      - Верхний порог времени
 *      - UUID клиента
 *      - Размер разового пакета
 */
struct TableTmpFromSrv : public StorageDDLOperations<SqlEngineSharedPtr_t> {
    static const QString TBL_NAME;

    static const QString FLD_ID;
    static const QString FLD_UUID;
    static const QString FLD_FULL_TEXT;

protected:
    // StorageElement interface
    virtual BoolVariantResult_t doCreate(const SqlEngine_t &engine);
    virtual BoolVariantResult_t doUpgrade(const SqlEngine_t &engine, const V1_t &fromVersion);
    virtual BoolVariantResult_t doUpgrade(const SqlEngine_t &engine, const V2_t &fromVersion);
    virtual BoolVariantResult_t doUpgrade(const SqlEngine_t &engine, const V3_t &fromVersion);
    virtual BoolVariantResult_t doUpgrade(const SqlEngine_t &engine, const V4_t &fromVersion);
};


} // namespace sqlite

} // namespace storage


#endif // SQLITESTORAGE_ELEMENTS_H
