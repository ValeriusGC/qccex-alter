#ifndef SQLITESTORAGE_H
#define SQLITESTORAGE_H

#include <QSqlDatabase>
#include <QSqlError>

#include "base_storage.h"
#include "note.h"
#include "author.h"

//namespace model {
//    QT_FORWARD_DECLARE_CLASS(Note)
//}

namespace storage {

using namespace model;

QT_FORWARD_DECLARE_CLASS(BaseTable)

class SqliteStorage : public BaseStorage
{
    Q_OBJECT
    typedef BaseStorage Inhherited_t;
public:
    explicit SqliteStorage(QObject *parent = 0);
    ~SqliteStorage();

    /**
     *  Returns list of table names, for information.
     * @brief tables
     * @return boolean result as 'result' and list of table names as 'data'.
     */
    BoolVariantResult_t tables() const;

    NoteListResult_t getNotes() const;

    AuthorListResult_t getAuthors() const;

protected:
    // BaseStorage interface
    virtual void doCreateItems();
    virtual BoolVariantResult_t doCheckVersion() const;
    virtual BoolResult_t doCreate();
    virtual BoolResult_t doUpgrade(qint32 oldVersion);
    virtual void doFetchNotes(qint64 id);
    virtual void doClearNotes(qint64 id);
    virtual void doAddNotes(qint64 id, const QSharedPointer<QObject> &notes);
    virtual void doFetchAuthors(qint64 id);

signals:    
    void fireTaskFinished();

public slots:

private:

    BoolResult_t upgradeFromV1();
    BoolResult_t upgradeFromV2();

};

class DbHelper Q_DECL_FINAL{

public:
DbHelper(const QString &name) {
    m_db = QSqlDatabase::database();
    if(!m_db.isOpen()) {
        m_db = QSqlDatabase::addDatabase("QSQLITE");
        m_db.setDatabaseName(name);
        if (!m_db.open()){
            m_lastError = m_db.lastError().text();
        }
    }
}

~DbHelper() {
    m_db.close();
}

QSqlDatabase db() const {
    return m_db;
}

QString lastError() const {
    return m_lastError;
}

private:
    QSqlDatabase m_db;
    QString m_lastError;

    DbHelper(DbHelper&);
    DbHelper &operator=(DbHelper);

};

}


#endif // SQLITESTORAGE_H
