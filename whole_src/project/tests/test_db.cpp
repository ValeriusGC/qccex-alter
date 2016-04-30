#include "test_db.h"

#include "qst/qstdbconnection.h"
#include "shared_result.h"

using namespace Qst;
using namespace vfx_shared;

void Test_DB::initTestCase()
{
    QstDBConnection conn;
    conn.setDriverName("QSQLITE");
    const BoolResult_t res = RES_TRUE;
}

void Test_DB::testVersion()
{
    QFAIL("NOT REALIZED YET");
}

void Test_DB::cleanupTestCase()
{

}
