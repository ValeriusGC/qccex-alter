#ifndef STORAGEELEMENT_H
#define STORAGEELEMENT_H

#include <QtCore>
#include <QSqlDatabase>
#include "storage_globals.h"


namespace storage {
namespace sqlite {

//using vfx_shared::BoolResult_t;
//using vfx_shared::BoolResult_t;

class IStorageOperations
{
public:
    static const int INVALID_VERSION;
    static const int NEW_VERSION;

    /**
     * This vDTR guarantees call of descendant's DTR. It can be useful if descendants will have resources to free.
     * @brief ~IStorageOperations
     */
    virtual ~IStorageOperations();

    BoolVariantResult_t create(QSqlDatabase db);
    BoolVariantResult_t upgrade(QSqlDatabase db, const qint32 &fromVersion);

protected:
    virtual BoolVariantResult_t doCreate(QSqlDatabase db) = 0;
    virtual BoolVariantResult_t doUpgrade(QSqlDatabase db, const V1_t &fromVersion) = 0;
    virtual BoolVariantResult_t doUpgrade(QSqlDatabase db, const V2_t &fromVersion) = 0;
};



class StorageElement
{
public:
    explicit StorageElement();
    ~StorageElement();

    static const int INVALID_VERSION;
    static const int NEW_VERSION;

    BoolResult_t init(QSqlDatabase db, qint32 oldVersion) Q_DECL_FINAL;

protected:
//    virtual IntResult_t doGetCurrentVersion(QSqlDatabase db) const = 0;
//    virtual BoolResult_t  doSetNewVersion(QSqlDatabase db) = 0;

    virtual BoolResult_t doCreate(QSqlDatabase db) = 0;
    virtual BoolResult_t doUpgradeFromV1(QSqlDatabase db) = 0;
    virtual BoolResult_t doUpgradeFromV2(QSqlDatabase db) = 0;

};

} // namespace sqlite
} // namespace storage

#endif // STORAGEELEMENT_H
