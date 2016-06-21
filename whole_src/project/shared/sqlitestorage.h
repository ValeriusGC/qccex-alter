#ifndef SQLITESTORAGE_H
#define SQLITESTORAGE_H

#include <QSqlDatabase>
#include <QSqlError>

#include "base_storage.h"
#include "sqlstorageelement.h"
#include "progressinfo.h"
#include "sqlengine.h"

using namespace nq;
#include "note.h"
#include "author.h"

//namespace model {
//    QT_FORWARD_DECLARE_CLASS(Note)
//}

namespace storage {

using namespace model;

QT_FORWARD_DECLARE_CLASS(BaseTable)

/**
 * @brief The SqliteStorage class
 */
class SqliteStorage : public BaseStorage<SqlEngineSharedPtr_t>
{
    Q_OBJECT
    typedef BaseStorage Inhherited_t;
    typedef Result<Notes *, QString>  NotesResult_t;
    typedef SqlEngineSharedPtr_t  SqlEngine_t;

public:

    /**
     * @brief instance (As Meyers singleton)
     * @return singleton.
     *
     */
    static SqliteStorage &instance();

    /**
     *  Returns list of table names, for information.
     * @brief tables
     * @return boolean result as 'result' and list of table names as 'data'.
     */
    BoolVariantResult_t tables() const;

//    NoteListResult_t getNotes() const;

//    AuthorListResult_t getAuthors() const;

    // IStorageOperations interface
    virtual BoolVariantResult_t doCreate(const SqlEngine_t &engine);
    virtual BoolVariantResult_t doUpgrade(const SqlEngine_t &engine, const V1_t &dummy);
    virtual BoolVariantResult_t doUpgrade(const SqlEngine_t &engine, const V2_t &dummy);
    virtual BoolVariantResult_t doUpgrade(const SqlEngine_t &engine, const V3_t &dummy);


protected:
    // BaseStorage interface
    virtual void doReset(SqlEngineSharedPtr_t engine);
    virtual EngineResult_t makeWritableEngine() const;
    virtual void doCreateItems(SqlEngineSharedPtr_t engine);
    virtual BoolVariantResult_t doCheckVersion(SqlEngineSharedPtr_t engine) const;
//    virtual BoolResult_t doCreate();
//    virtual BoolResult_t doUpgrade(qint32 oldVersion);
    virtual void doFetchNotes(qint64 taskId, const UuidVector_t &ids);
    virtual void doClearNotes(qint64 taskId);
    virtual void doAddNotes(qint64 taskId, const QSharedPointer<model::Notes> &notes);
    virtual void doEditNotes(qint64 taskId, const QSharedPointer<model::Notes> &notes);
    virtual void doMarkNotesAsDeleted(qint64 taskId, const UuidVector_t &ids, bool setMark);
    virtual void doRemoveNotes(qint64 taskId, const UuidVector_t &ids);
    virtual void doFetchAuthors(qint64 taskId);

signals:    
    void fireTaskFinished();

public slots:

private:
    typedef QSharedPointer < StorageDDLOperations <SqlEngineSharedPtr_t> > StoragePtr_t;
    QMap< QString, StoragePtr_t > m_tableMap;


    explicit SqliteStorage(QObject *parent = 0);
    ~SqliteStorage();
    SqliteStorage(SqliteStorage &);
    SqliteStorage &operator =(SqliteStorage &);


    /**
     *  Inner fetching
     * @brief _fetch
     * @param id
     * @return
     */
    NotesResult_t _fetch(QSqlDatabase db, const UuidVector_t &ids);

};

}


#endif // SQLITESTORAGE_H
