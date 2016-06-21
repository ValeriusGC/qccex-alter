#include <QSqlError>

#include "sqlengine.h"
#include "shared_def.h"

using namespace storage;

qint32 SqlEngine::m_testCounter = 0;

SqlEngine::~SqlEngine()
{
    if(m_withTransaction) {
        if(m_innerState.result()) {
//            LOG_TP("m_db.commit()");
            m_db.commit();
        }else{
//            LOG_TP("m_db.rollback()");
            m_db.rollback();
        }
    }
    m_db.close();
//    LOG_TP(--m_testCounter);
    DEC_THIS(false);
}

QSqlDatabase SqlEngine::db() const {
    return m_db;
}

BoolVariantResult_t &SqlEngine::innerState() {
    return m_innerState;
}

SqlEngine *SqlEngine::makeReadable(const QString &name)
{
    return new SqlEngine(name, false);
}

SqlEngine *SqlEngine::makeWritable(const QString &name)
{
    return new SqlEngine(name, true);
}

SqlEngine::SqlEngine(const QString &name, bool withTransaction)
    : m_withTransaction(withTransaction),
      m_innerState(false, QVariant())
{
//    LOG_TP(++m_testCounter);
    INC_THIS(false);
    m_db = QSqlDatabase::database();
    if(!m_db.isOpen()) {
        m_db = QSqlDatabase::addDatabase("QSQLITE");
        m_db.setDatabaseName(name);
        if (!m_db.open()){
            m_innerState = {false, m_db.lastError().text()};
        }
    }

    if(m_withTransaction) {
        if(m_db.isOpen()) {
            if(!m_db.transaction()) {
//                LOG_TP("TRANSACTION IS FALSE");
                m_innerState = {false, m_db.lastError().text()};
            }else{
//                LOG_TP("TRANSACTION IS ON");
                m_innerState = {true, QVariant()};
            }
        }
    }else{ // if(m_transaction)
        m_innerState = {true, QVariant()};
    }
}

