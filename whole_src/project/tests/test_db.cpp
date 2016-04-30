#include "test_db.h"

#include "qst/qstdbconnection.h"
#include "shared_result.h"

using namespace Qst;
using namespace vfx_shared;

void Test_DB::initTestCase()
{
    m_dbName = "testDB.db";
    // Create test-BD with particular version
    QstDBConnection conn;
    conn.setDriverName("QSQLITE");
    conn.setDatabaseName(m_dbName);
    const BoolResult_t res = conn.test();
    QVERIFY2(res.result(), res.message().toLatin1());
}

void Test_DB::testVersion()
{
    QFAIL("NOT REALIZED YET");
}

void Test_DB::cleanupTestCase()
{
    // Remove test-BD with particular version
}
