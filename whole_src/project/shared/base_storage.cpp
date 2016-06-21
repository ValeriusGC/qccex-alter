#include <QSharedPointer>

#include "base_storage.h"
#include "storage_globals.h"

using namespace storage;
using nq::ProgressInfo;

StorageObjectOperations::StorageObjectOperations(QObject *parent)
    : Inherited_t(parent),
      m_name(""),
      m_isInit(false)

{

}

StorageObjectOperations::~StorageObjectOperations()
{

}

QString StorageObjectOperations::name() const
{
    return m_name;
}

void StorageObjectOperations::init(const QString &name)
{
    m_name = name;
    const qint32 operId = 10;

    // Call doInit() from descendants.
    BoolVariantResult_t res = doInit();

    m_isInit = res.result();
    ProgressInfo::Status_e ps;
    if(m_isInit) {
        ps = ProgressInfo::TPS_Success;
    }else{
        ps = ProgressInfo::TPS_Error;
    }
    emit fireInitProgress({operId, ps, 0, 0, 0, res.data().toString()});
    return;
}

bool StorageObjectOperations::isInit() const
{
    return m_isInit;
}

void StorageObjectOperations::fetchNotes(qint64 taskId, const UuidVector_t &ids)
{
    doFetchNotes(taskId, ids);
}

void StorageObjectOperations::clearNotes(qint64 taskId)
{
    doClearNotes(taskId);
}

void StorageObjectOperations::addNotes(qint64 taskId, const QSharedPointer<model::Notes> &notes)
{
    doAddNotes(taskId, notes);
}

void StorageObjectOperations::editNotes(qint64 taskId, const QSharedPointer<model::Notes> &notes)
{
    doEditNotes(taskId, notes);
}

void StorageObjectOperations::markNotesAsDeleted(qint64 taskId, const UuidVector_t &ids, bool setMark)
{
    doMarkNotesAsDeleted(taskId, ids, setMark);
}

void StorageObjectOperations::removeNotes(qint64 taskId, const UuidVector_t &ids)
{
    doRemoveNotes(taskId, ids);
}

void StorageObjectOperations::fetchAuthors(qint64 taskId)
{
    doFetchAuthors(taskId);
}

