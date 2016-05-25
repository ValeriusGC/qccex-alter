#ifndef TEST_COMMON_DEFS
#define TEST_COMMON_DEFS

#include <QFile>

namespace test {

static const QString TEST_DB_NAME   = "testDB.db";
static const QString TEST_DB_CONN   = "default";
static const qint32 TEST_INVALID_NUMBER = -1;
//static const qint32 TEST_DB_V0          = 0;
static const qint32 TEST_DB_V1          = 1;
static const qint32 TEST_DB_V2          = 2;
static const qint32 TEST_DB_V3          = 3;



/**
 *  Simply deletes db-file.
 * @brief deleteDB
 * @return
 */
static bool deleteDB(const QString &name) {
    QFile f(name);
    if(!f.exists()) {
        return true;
    }
    return f.remove();
}


}



#endif // TEST_COMMON_DEFS

