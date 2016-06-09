#ifndef SQLITESTORAGE_H
#define SQLITESTORAGE_H

#include <QSqlDatabase>
#include <QSqlError>

#include "base_storage.h"
#include "sqlstorageelement.h"
#include "progressinfo.h"
using namespace nq;
#include "note.h"
#include "author.h"

//namespace model {
//    QT_FORWARD_DECLARE_CLASS(Note)
//}

namespace storage {

using namespace model;

QT_FORWARD_DECLARE_CLASS(BaseTable)

class SqliteStorage : public BaseStorage, public sqlite::IStorageOperations
{
    Q_OBJECT
    typedef BaseStorage Inhherited_t;
    typedef Result<Notes *, QString>  NotesResult_t;

public:
    explicit SqliteStorage(QObject *parent = 0);
    ~SqliteStorage();

    /**
     *  Returns list of table names, for information.
     * @brief tables
     * @return boolean result as 'result' and list of table names as 'data'.
     */
    BoolVariantResult_t tables() const;

//    NoteListResult_t getNotes() const;

//    AuthorListResult_t getAuthors() const;

    // IStorageOperations interface
    virtual BoolVariantResult_t doCreate(QSqlDatabase db);
    virtual BoolVariantResult_t doUpgrade(QSqlDatabase db, const V1_t &dummy);
    virtual BoolVariantResult_t doUpgrade(QSqlDatabase db, const V2_t &dummy);




protected:
    // BaseStorage interface
    virtual void doCreateItems();
    virtual BoolVariantResult_t doCheckVersion() const;
    virtual BoolResult_t doCreate();
    virtual BoolResult_t doUpgrade(qint32 oldVersion);
    virtual void doFetchNotes(qint64 id);
    virtual void doClearNotes(qint64 id);
//    virtual void doAddNotes(qint64 id, const QSharedPointer<QObject> &notes);
//    virtual void doAddNotes2(qint64 id, const QVector<QSharedPointer<model::Note>> &notes);
    virtual void doAddNotes3(qint64 id, const QSharedPointer<model::Notes> &notes);
    virtual void doMarkNotesAsDeleted(qint64 id, const QVector<qint32> &ids);
    virtual void doRemoveNotes(qint64 id, const QVector<qint32> &ids);
    virtual void doFetchAuthors(qint64 id);

signals:    
    void fireTaskFinished();

public slots:

private:
    typedef QSharedPointer <IStorageOperations> StoragePtr_t;
    QMap< QString, StoragePtr_t > m_tableMap;

    /**
     *  Inner fetching
     * @brief _fetch
     * @param id
     * @return
     */
    NotesResult_t _fetch(QSqlDatabase db);

    BoolResult_t upgradeFromV1();
    BoolResult_t upgradeFromV2();

};

class DbHelper Q_DECL_FINAL{

public:
DbHelper(const QString &name) : m_result(false, QVariant()){
    LOG_T;
    m_db = QSqlDatabase::database();
    if(!m_db.isOpen()) {
        m_db = QSqlDatabase::addDatabase("QSQLITE");
        m_db.setDatabaseName(name);
        if (!m_db.open()){
            m_result = {false, m_db.lastError().text()};
        }
    }

    if(m_db.isOpen()) {
        if(!m_db.transaction()) {
            m_result = {false, m_db.lastError().text()};
        }else{
            LOG_TP("TRANSACTION IS ON" << ++m_counter);
            m_result = {true, QVariant()};
        }
    }

}

~DbHelper() {
    if(m_result.result()) {
        bool ok = m_db.commit();
        LOG_TP("COMMIT" << ok << --m_counter);
    }else{
        bool ok = m_db.rollback();
        LOG_TP("ROLLBACK" << ok << --m_counter);
    }
//    if(!m_committed){
//        const bool ok = m_db.rollback();
//        LOG_TP("ROLLBACK" << ok);
//    }
    m_db.close();
}

QSqlDatabase db() const {
    return m_db;
}

//QString lastError() const {
//    return m_lastError;
//}

//void commit() {
//    if(!m_committed) {
//        m_committed = m_db.commit();
//        LOG_TP("COMMITTED" << m_committed);
//    }
//}

BoolVariantResult_t &result() {
    return m_result;
}

private:
//    bool m_committed;
    QSqlDatabase m_db;
//    QString m_lastError;
    static qint32 m_counter;
    BoolVariantResult_t m_result;

    DbHelper(DbHelper&);
    DbHelper &operator=(DbHelper);

};

}


#endif // SQLITESTORAGE_H
