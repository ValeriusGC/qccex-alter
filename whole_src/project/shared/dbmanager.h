#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QtSql>

#include "shared_result.h"

class DbManager : public QObject
{
    Q_OBJECT
public:
//    explicit DbManager(const QString &name,  QObject *parent = 0);
//    ~DbManager();

    /**
     * @brief touch
     * @param name
     * @return version before init (because init may change it).
     */
    static vfx_shared::Result<QSqlError, quint32> touch(const QString &name);

    static vfx_shared::Result<QSqlError, DbManager*> init(const QString &name);


    /**
     * @brief init
     * @return error or NoError;
     *
     * Duplicated init is controlling by code.
     */
    QSqlError init();

    /**
     * @brief release
     *
     * Duplicated release is controlling by code.
     */
    void release();


signals:

public slots:

private:
//    QSqlDatabase *m_db;
    QString m_name;
    const QString m_dbConnName;

    // Creators
    explicit DbManager(const QString &name,  QObject *parent = 0);
    ~DbManager();
    DbManager(DbManager &);
    DbManager &operator =(DbManager &);
    // ~Creators

};

#endif // DBMANAGER_H
