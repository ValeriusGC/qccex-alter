#ifndef SQLENGINE_H
#define SQLENGINE_H

#include <QtCore>
#include <QSqlDatabase>

#include "shared_result.h"
#include "storage_globals.h"

namespace storage {

/**
 * @brief The DbHelper class
 */
class SqlEngine Q_DECL_FINAL
{

public:
    ~SqlEngine();
    QSqlDatabase db() const;
    BoolVariantResult_t &innerState();

    static SqlEngine *makeReadable(const QString &name);
    static SqlEngine *makeWritable(const QString &name);

private:
    QSqlDatabase m_db;
    bool m_withTransaction;
    static qint32 m_testCounter;
    BoolVariantResult_t m_innerState;

    SqlEngine(const QString &name, bool withTransaction);
    SqlEngine(SqlEngine&);
    SqlEngine &operator=(SqlEngine);

};

typedef QSharedPointer<SqlEngine> SqlEngineSharedPtr_t;

} // namespace storage

#endif // SQLENGINE_H
