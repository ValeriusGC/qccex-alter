#include <QDebug>

#include "dbmanager.h"
#include "shared_def.h"

using namespace vfx_shared;

DbManager::DbManager(const QString &name, QObject *parent)
    : QObject(parent),
      m_dbConnName("default")
{
    Q_ASSERT_X(name != nullptr, Q_FUNC_INFO, "database name must be set to nonzero");
    m_name = name != "" ? name : ":memory:";
    LOG_T;
    INC_THIS(true);
}

DbManager::~DbManager()
{
    LOG_T;
    release();
    DEC_THIS(true);
}

vfx_shared::Result<QSqlError, quint32> DbManager::touch(const QString &name)
{

}

vfx_shared::Result<QSqlError, DbManager *> DbManager::init(const QString &name/*, quint32 version*/)
{
    DbManager *dbm = new DbManager(name);
    QSqlError res = dbm->init();
    return {res, dbm};
}

QSqlError DbManager::init()
{
    QSqlDatabase db = QSqlDatabase::database(m_dbConnName, false);
    if(!db.isOpen()) {
        db = QSqlDatabase::addDatabase("QSQLITE", m_dbConnName);
        db.setDatabaseName(m_name);
        if (!db.open()){
            return db.lastError();
        }
    }else{
        LOG_TP("duplicated init()");
    }
    return QSqlError();
}

void DbManager::release()
{
    QSqlDatabase db = QSqlDatabase::database(m_dbConnName, false);
    if(db.isOpen()) {
        LOG_TP(db.isOpen());
        db.close();
    }
}

