#include "sqlstorageelement.h"

namespace storage {
namespace sqlite {

const int IStorageOperations::INVALID_VERSION = 0;
const int IStorageOperations::NEW_VERSION = 3;

IStorageOperations::~IStorageOperations()
{

}

BoolVariantResult_t IStorageOperations::create(QSqlDatabase db)
{
    return doCreate(db);
}

BoolVariantResult_t IStorageOperations::upgrade(QSqlDatabase db, const qint32 &fromVersion)
{
    switch (fromVersion) {
    case 1:
        return doUpgrade(db, V1_t());
    case 2:
        return doUpgrade(db, V2_t());
    default:
        break;
    }
    return {false, QVariant()};
}

//======================================================================================================================

const int StorageElement::INVALID_VERSION = 0;
const int StorageElement::NEW_VERSION = 3;

StorageElement::StorageElement()
{

}

StorageElement::~StorageElement()
{

}

BoolResult_t StorageElement::init(QSqlDatabase db, qint32 oldVersion)
{
//    IntResult_t ver = doGetCurrentVersion(db);
//    if(ver.result() == INVALID_VERSION) {
//        return {false, ver.data()};
//    }

//    const qint32 currV = ver.result();
    BoolResult_t res = {false, ""};
    switch(oldVersion){
    case INVALID_VERSION:
        res = doCreate(db);
        break;
    case 1:
        res = doUpgradeFromV1(db);
        break;
    case 2:
        res = doUpgradeFromV2(db);
        break;
    case NEW_VERSION:
        res = {true, ""};
        break;
    default:
        break;
    }

//    if(res.result()){
//        res = doSetNewVersion(db);
//    }

    return res;
}

} // namespace sqlite
} // namespace storage

