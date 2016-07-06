#ifndef STORAGEELEMENT_H
#define STORAGEELEMENT_H

#include <QtCore>
#include <QSqlDatabase>
#include "storage_globals.h"

namespace storage {

template<typename Engine>
class StorageDDLOperations
{
public:
    static const int INVALID_VERSION;
    static const int NEW_VERSION;

    /**
     * This vDTR guarantees call of descendant's DTR. It can be useful if descendants will have resources to free.
     * @brief ~IStorageOperations
     */
    virtual ~StorageDDLOperations();

    BoolVariantResult_t create(const Engine &engine);
    BoolVariantResult_t upgrade(const Engine &engine, const qint32 &fromVersion);

protected:
    virtual BoolVariantResult_t doCreate(const Engine &engine) = 0;
    virtual BoolVariantResult_t doUpgrade(const Engine &engine, const V1_t &fromVersion) = 0;
    virtual BoolVariantResult_t doUpgrade(const Engine &engine, const V2_t &fromVersion) = 0;
    virtual BoolVariantResult_t doUpgrade(const Engine &engine, const V3_t &fromVersion) = 0;
    virtual BoolVariantResult_t doUpgrade(const Engine &engine, const V4_t &fromVersion) = 0;
};

template<typename Engine>
const int StorageDDLOperations<Engine>::INVALID_VERSION = 0;
//const int IStorageOperations::NEW_VERSION = 3;
//  16/06/11
// VERSION 4: BASE64 UUID as ids
//template<typename Engine>
//const int StorageDDLOperations<Engine>::NEW_VERSION = 4;
//  16/06/21
// VERSION 5: SynchTable for temporarily getting records from server.
//              After this loooong operation one can quick place them into local tables.
template<typename Engine>
const int StorageDDLOperations<Engine>::NEW_VERSION = 5;



template<typename Engine>
StorageDDLOperations<Engine>::~StorageDDLOperations()
{

}

template<typename Engine>
BoolVariantResult_t StorageDDLOperations<Engine>::create(const Engine &engine)
{
    return doCreate(engine);
}

template<typename Engine>
BoolVariantResult_t StorageDDLOperations<Engine>::upgrade(const Engine &engine, const qint32 &fromVersion)
{
    switch (fromVersion) {
    case 1:
        return doUpgrade(engine, V1_t());
    case 2:
        return doUpgrade(engine, V2_t());
    case 3:
        return doUpgrade(engine, V3_t());
    case 4:
        return {true, QVariant()};
    default:
        return {false, QObject::tr("Unknown version '%1'").arg(fromVersion)};
    }
}

} // namespace storage

#endif // STORAGEELEMENT_H
