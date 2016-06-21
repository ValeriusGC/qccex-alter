#include "sqlstorageelement.h"

namespace storage {

//template<typename Engine>
//const int IStorageOperations<Engine>::INVALID_VERSION = 0;
////const int IStorageOperations::NEW_VERSION = 3;
////  16/06/11
//// VERSION 4: BASE64 UUID as ids
//template<typename Engine>
//const int IStorageOperations<Engine>::NEW_VERSION = 4;

//template<typename Engine>
//IStorageOperations<Engine>::~IStorageOperations()
//{

//}

//template<typename Engine>
//BoolVariantResult_t IStorageOperations<Engine>::create(Engine engine)
//{
//    return doCreate(engine);
//}

//template<typename Engine>
//BoolVariantResult_t IStorageOperations<Engine>::upgrade(Engine engine, const qint32 &fromVersion)
//{
//    switch (fromVersion) {
//    case 1:
//        return doUpgrade(engine, V1_t());
//    case 2:
//        return doUpgrade(engine, V2_t());
//    case 3:
//        return doUpgrade(engine, V3_t());
//    default:
//        break;
//    }
//    return {false, QVariant()};
//}

} // namespace storage

