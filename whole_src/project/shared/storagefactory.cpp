#include "storagefactory.h"
#include "sqlitestorage.h"

using namespace storage;

StorageFactory &StorageFactory::instance()
{
    static StorageFactory singleton;
    return singleton;
}

BaseStorage *StorageFactory::make(const QString &type)
{
    BaseStorage *storage = nullptr;
    if(type=="sqlite"){
        storage = m_map.value(type);
        if(!storage){
            storage = new SqliteStorage(this);
            m_map[type] = storage;
        }
    }
    return storage;
}

StorageFactory::StorageFactory() : Inherited_t(nullptr)
{
    INC_THIS(false);
}

StorageFactory::~StorageFactory()
{
    DEC_THIS(false);
}

