#ifndef TESTSQLITESTORAGE_H
#define TESTSQLITESTORAGE_H

#include <QObject>
#include <QSignalSpy>

#include "alltests.h"
#include "progressinfo.h"
using namespace nq;
#include "storage_globals.h"
#include "sqlitestorage.h"

using namespace storage;

class TestSqliteStorage : public QObject
{
    Q_OBJECT
public:

signals:

public slots:

private slots:
    // Auto-initializator for entire object
    void initTestCase();
    // Auto-cleaner for entire object
    void cleanupTestCase();
    // Auto-initializator for every test-function
    void init();
    // Auto-cleaner for every test-function
    void cleanup();

    /**
     *  Test creating DB.v.3
     * @brief testCreateV3
     */
    void testCreateV3();

    /**
     *  Updates from V1 to V3
     * @brief testUpgradeV1V3
     */
    void testUpgradeV1V3();

    /**
     *  Updates from V2 to V3
     * @brief testUpgradeV2V3
     */
    void testUpgradeV2V3();

private:
    /**
     *  Time to init
     * @brief m_initTime
     */
    qint32 m_initTime;

    QString m_dbName;

    storage::SqliteStorage *m_storage;

    /**
     * @brief createV1
     * @return result of creation with error-string (if not).
     */
    BoolVariantResult_t createV1();

    /**
     * @brief createV2
     * @return result of creation with error-string (if not).
     */
    BoolVariantResult_t createV2();

};

DECLARE_TEST(TestSqliteStorage)

#endif // TESTSQLITESTORAGE_H
