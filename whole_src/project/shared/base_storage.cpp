#include <QSharedPointer>

#include "base_storage.h"
#include "storage_globals.h"

using namespace storage;
using nq::ProgressInfo;

const qint32 BaseStorage::NEW_VERSION = storage::STORAGE_CURR_VER;

BaseStorage::BaseStorage(QObject *parent)
    : QObject(parent),
      m_name(""),
      m_isInit(false)
{

}

BaseStorage::~BaseStorage()
{

}

QString BaseStorage::name() const
{
    return m_name;
}

void BaseStorage::init(const QString &name)
{
    // Just in this moment we check version too and upgrade it if needed.

    m_name = name;
    const qint64 id = 0;
    const qint32 max = 5;

    emit fireInitProgress({id, ProgressInfo::TPS_Progress, 0, max, 1, tr("making tables", "DB")});
    doCreateItems();
    emit fireInitProgress({id, ProgressInfo::TPS_Progress, 0, max, 2, tr("check version", "DB")});
    BoolVariantResult_t curr = doCheckVersion();

    BoolResult_t res = {true, QString()};

    if(curr.result() == false) {
        emit fireInitProgress({id, ProgressInfo::TPS_Progress, 0, max, 3, tr("creating", "DB")});
        res = doCreate();
    }else{
        qint32 oldVersion = curr.data().toInt();
        if(oldVersion != NEW_VERSION) {
            res = doUpgrade(oldVersion);
        }
    }

    if(res.result() == true) {
        // In this case all is OK
        m_isInit = true;
        emit fireInitProgress(ProgressInfo());
    }else{
        emit fireInitProgress({id, ProgressInfo::TPS_Error, 0, max, max, res.data()});
    }

//    emit fireInitProgress({ProgressInfo::TPS_Error, 0, 0, 0, ""});
}

bool BaseStorage::isInit() const
{
    return m_isInit;
}

BoolVariantResult_t BaseStorage::version() const
{
    if(!m_isInit) {
        return {false, tr("Storage not initialized yet.")};
    }

    return {true, NEW_VERSION};
}

void BaseStorage::fetchNotes(qint64 id)
{
    return doFetchNotes(id);
}

void BaseStorage::clearNotes(qint64 id)
{
    doClearNotes(id);
}

//void BaseStorage::addNotes(qint64 id, const QSharedPointer<QObject> &notes)
//{
//    doAddNotes(id, notes);
//}

//void BaseStorage::addNotes2(qint64 id, const QVector<QSharedPointer<model::Note> > &notes)
//{
//    doAddNotes2(id, notes);
//}

void BaseStorage::addNotes3(qint64 id, const QSharedPointer<model::Notes> &notes)
{
    doAddNotes3(id, notes);
}

void BaseStorage::markNotesAsDeleted(qint64 id, const QVector<qint32> &ids)
{
    doMarkNotesAsDeleted(id, ids);
}

void BaseStorage::removeNotes(qint64 id, const QVector<qint32> &ids)
{
    doRemoveNotes(id, ids);
}

void BaseStorage::fetchAuthors(qint64 id)
{
    doFetchAuthors(id);
}

