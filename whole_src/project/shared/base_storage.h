#ifndef BASESTORAGE_H
#define BASESTORAGE_H

#include <QObject>
#include <QSharedPointer>

//#include "sb_def.h"

#include "sqlstorageelement.h"
#include "storage_globals.h"
//using namespace storage;

#include "basemodelitemlist.h"

#include "progressinfo.h"

#include "note.h"

namespace storage {

// ID for operations
#define TASK_ID_ADD_NOTES                   101
#define TASK_ID_DEL_NOTES                   102
#define TASK_ID_EDIT_NOTES                  103
#define TASK_ID_MARK_NOTES_AS_DEL           104
#define TASK_ID_FETCH_NOTES                 105
#define TASK_ID_CLEAR_NOTES                 106
// from this we can apply user ID (TASK_ID_USER+1, etc)
#define TASK_ID_USER                        10000

class StorageObjectOperations : public QObject
{
    Q_OBJECT
    typedef QObject Inherited_t;
public:
    explicit StorageObjectOperations(QObject *parent = 0);
    ~StorageObjectOperations();

    /**
     * @brief name
     * @return Storage name. It is empty before init().
     */
    QString name() const Q_DECL_FINAL;

    void init(const QString &name) Q_DECL_FINAL;

    bool isInit() const Q_DECL_FINAL;

    /**
     * @brief isBusy
     *  Indicates that Storage busy.
     * @return Busy state.
     */
    bool isBusy() const Q_DECL_FINAL;

    void fetchNotes(qint64 taskId, const UuidVector_t &ids) Q_DECL_FINAL;
    void clearNotes(qint64 taskId) Q_DECL_FINAL;
    void addNotes(qint64 taskId, const QSharedPointer<model::Notes> &notes) Q_DECL_FINAL;
    void editNotes(qint64 taskId, const QSharedPointer<model::Notes> &notes) Q_DECL_FINAL;
    void markNotesAsDeleted(qint64 taskId, const UuidVector_t &ids, bool setMark);
    void removeNotes(qint64 taskId, const UuidVector_t &ids) Q_DECL_FINAL;
    void fetchAuthors(qint64 taskId) Q_DECL_FINAL;


protected:
    virtual BoolVariantResult_t doInit() = 0;
    virtual void doFetchNotes(qint64 taskId, const UuidVector_t &ids) = 0;
    virtual void doClearNotes(qint64 taskId) = 0;
    virtual void doAddNotes(qint64 taskId, const QSharedPointer<model::Notes> &notes) = 0;
    virtual void doEditNotes(qint64 taskId, const QSharedPointer<model::Notes> &notes) = 0;
    virtual void doMarkNotesAsDeleted(qint64 taskId, const UuidVector_t  &ids, bool setMark) = 0;
    virtual void doRemoveNotes(qint64 taskId, const UuidVector_t &ids) = 0;
    virtual void doFetchAuthors(qint64 taskId) = 0;

signals:
    void fireInitProgress(const nq::ProgressInfo &pi);
    void fireTaskProgress(const nq::ProgressInfo &pi, const QVariant &sp);
    void fireTaskNoteProgress(const nq::ProgressInfo &pi, const QSharedPointer<model::Notes> &notes);

    void fireNotesAdded(const QVector<qint32> &ids);
    void fireNotesMarkedAsDeleted(const QVector<qint32> &ids);


private:
    QString m_name;
    bool m_isInit;
};



template<typename Engine>
class BaseStorage : public StorageObjectOperations, public StorageDDLOperations<Engine>
{
public:
    explicit BaseStorage(QObject *parent = 0);
    ~BaseStorage();

//    QString name() const;

//    void init(const QString &name) Q_DECL_FINAL;

//    bool isInit() const Q_DECL_FINAL;

//    /**
//     *  Returns version, that is stored in.
//     * @brief version
//     * @return current version
//     */
//    BoolVariantResult_t version() const Q_DECL_FINAL;

//    void fetchNotes(qint64 id) Q_DECL_FINAL;
//    void clearNotes(qint64 id) Q_DECL_FINAL;
////    void addNotes(qint64 id, const QSharedPointer<QObject> &notes) Q_DECL_FINAL;
////    void addNotes2(qint64 id, const QVector<QSharedPointer<model::Note>> &notes) Q_DECL_FINAL;
//    void addNotes3(qint64 id, const QSharedPointer<model::Notes> &notes) Q_DECL_FINAL;
//    void markNotesAsDeleted(qint64 id, const QVector<qint32> &ids, bool setMark);

//    void removeNotes(qint64 id, const QVector<qint64> &ids) Q_DECL_FINAL;

//    void fetchAuthors(qint64 id) Q_DECL_FINAL;

protected:

    /**
     * Used internal by descendants in init() and doCheckVersion();
     * @brief ACTUAL_VERSION
     */
//    static const qint32 NEW_VERSION;

    typedef Result<Engine, QString> EngineResult_t;

    // StorageObjectOperations
    virtual BoolVariantResult_t doInit();

    /**
     * @brief makeWritableEngine
     * @return Engine-object for operation or nullptr + String as debug-message if nullptr;
     */
    virtual EngineResult_t makeWritableEngine() const = 0;


    /**
     * @brief doReset
     * Resetting object in case of repeat init().
     */
    virtual void doReset(Engine engine) = 0;

    /**
     *  Creates storage items
     * @brief doCreateItems
     */
    virtual void doCreateItems(Engine engine) = 0;

    /**
     *  Checks current version of storage.
     * @brief doCheckVersion
     * @return Current version or NO_VERSION with info-string.
     */
//    virtual void doFetchNotes(qint64 id);
//    virtual void doClearNotes(qint64 id);
//    virtual void doAddNotes3(qint64 id, const QSharedPointer<model::Notes> &notes) = 0;
//    virtual void doMarkNotesAsDeleted(qint64 id, const QVector<qint32> &ids, bool setMark) = 0;
//    virtual void doRemoveNotes(qint64 id, const QVector<qint64> &ids) = 0;
//    virtual void doFetchAuthors(qint64 id) = 0;

//    virtual void doFetchNotes(qint64 id) = 0;
//    virtual void doClearNotes(qint64 id) = 0;
//    virtual void doAddNotes3(qint64 id, const QSharedPointer<model::Notes> &notes) = 0;
//    virtual void doMarkNotesAsDeleted(qint64 id, const QVector<qint32> &ids, bool setMark) = 0;
//    virtual void doRemoveNotes(qint64 id, const QVector<qint64> &ids) = 0;
//    virtual void doFetchAuthors(qint64 id) = 0;

    // For
    virtual BoolVariantResult_t doCheckVersion(Engine engine) const = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

using nq::ProgressInfo;

template<typename Engine>
BaseStorage<Engine>::BaseStorage(QObject *parent)
    : StorageObjectOperations(parent)
{

}

template<typename Engine>
BaseStorage<Engine>::~BaseStorage()
{

}

template<typename Engine>
BoolVariantResult_t BaseStorage<Engine>::doInit()
{
    const qint32 max = 5;
    // We need engine to make all operations
    EngineResult_t engineRes = makeWritableEngine();
    if(engineRes.result() == nullptr) {
        emit fireInitProgress({0, ProgressInfo::TPS_Error, 0, max, max, engineRes.data()});
        return {false, engineRes.data()};
    }
    Engine engine = engineRes.result();

    // Do reset in cse of repeat init.
    doReset(engine);

    // Just in this moment we check version too and upgrade it if needed.

    emit fireInitProgress({0, ProgressInfo::TPS_Progress, 0, max, 1, tr("making tables", "DB")});
    doCreateItems(engine);
    emit fireInitProgress({0, ProgressInfo::TPS_Progress, 0, max, 2, tr("check version", "DB")});


    // Check if this storage exists
    BoolVariantResult_t currOperRes = doCheckVersion(engineRes.result());
    if(currOperRes.result() == false) {
        // It means that VERSIOn not exists and we have to create new storage.
        emit fireInitProgress({0, ProgressInfo::TPS_Progress, 0, max, 3, tr("creating", "DB")});
        currOperRes = doCreate(engine);
    }else{
        const qint32 oldVersion = currOperRes.data().toInt();
        currOperRes = upgrade(engine, oldVersion);
    }

    return currOperRes;

}

//void BaseStorage::doFetchNotes(qint64 id)
//{

//}

//template<typename Engine>
//void BaseStorage<Engine>::fetchNotes(qint64 taskid)
//{
//    return doFetchNotes(taskid);
//}

//template<typename Engine>
//void BaseStorage<Engine>::clearNotes(qint64 id)
//{
//    doClearNotes(id);
//}

//void BaseStorage::addNotes(qint64 id, const QSharedPointer<QObject> &notes)
//{
//    doAddNotes(id, notes);
//}

//void BaseStorage::addNotes2(qint64 id, const QVector<QSharedPointer<model::Note> > &notes)
//{
//    doAddNotes2(id, notes);
//}

//template<typename Engine>
//void BaseStorage<Engine>::addNotes3(qint64 id, const QSharedPointer<model::Notes> &notes)
//{
//    doAddNotes3(id, notes);
//}

//template<typename Engine>
//void BaseStorage<Engine>::markNotesAsDeleted(qint64 id, const QVector<qint32> &ids, bool setMark)
//{
//    doMarkNotesAsDeleted(id, ids, setMark);
//}

//template<typename Engine>
//void BaseStorage<Engine>::removeNotes(qint64 id, const QVector<qint64> &ids)
//{
//    doRemoveNotes(id, ids);
//}

//template<typename Engine>
//void BaseStorage<Engine>::fetchAuthors(qint64 id)
//{
//    doFetchAuthors(id);
//}

} // namespace storage

#endif // BASESTORAGE_H
