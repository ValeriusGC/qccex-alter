#include <QSqlError>
#include <QSignalSpy>
#include <QSqlDatabase>

#include "test_db.h"
#include "test_common_defs.h"
using namespace test;
#include "shared_result.h"
using namespace vfx_shared;

//void Test_DB::initTestCase()
//{
//    m_dbName = "testDB.db";
////    Result<QSqlError, DbManager*> dbmRes = DbManager::init("testdb.db");
////    m_dbManager = dbmRes.message();
////    QSqlError sqlResult = dbmRes.result();
////    QVERIFY2(sqlResult.type() == QSqlError::NoError, sqlResult.text().toLatin1());
////    QSqlError sqlResult2 = m_dbManager->init();
//    //    QVERIFY2(sqlResult2.type() == QSqlError::NoError, sqlResult2.text().toLatin1());
//}

//void Test_DB::cleanupTestCase()
//{
////    m_dbManager->release();
//    //    delete m_dbManager;
//}

void Test_DB::init()
{
//    m_dbName = "testDB.db";
//    m_storage = nullptr;
//    // Checks that this QSqlDatabase is not exists.
//    QSqlDatabase db = QSqlDatabase::database(TEST_DB_CONN, false);
//    QVERIFY(db.isOpen() == false);

//    QVERIFY(m_spies.count() == 0);
}

void Test_DB::cleanup()
{
//    storage::STORAGE_CURR_VER = STORAGE_V1;
//    // Deletes physical file;
//    QVERIFY(deleteDB());
//    // Deletes storage (and QSqlDatabase)
//    delete m_storage;

//    foreach (auto s, m_spies) {
//        delete s;
//    }
//    m_spies.clear();
}

void Test_DB::testCreateWithInvalidVersionZero()
{
//    m_storage = new Storage();
//    m_spies.append(new QSignalSpy(m_storage, SIGNAL(fireOnError(QString))));
//    m_spies.append(new QSignalSpy(m_storage, SIGNAL(fireOnCreate(QString,qint32))));
//    m_spies.append(new QSignalSpy(m_storage, SIGNAL(fireOnUpgrade(QString,qint32,qint32))));

//    QVERIFY(m_storage->isInit() == false);
//    QVERIFY((qint32)m_storage->version().result().type() != (qint32)QSqlError::NoError);
//    QCOMPARE(m_storage->version().data(), TEST_INVALID_NUMBER);
//    // When pass ZERO_VER wait TEST_INVALID_NUMBER

//    storage::STORAGE_CURR_VER = STORAGE_V1;
//    m_storage->init(m_dbName);
//    // Wait some time for initialization
//    QTest::qWait(200);

//    QCOMPARE(m_spies.at(SS_OnError)->count(), 1);
//    QCOMPARE(m_spies.at(SS_OnCreate)->count(), 0);
//    QCOMPARE(m_spies.at(SS_OnUpgrade)->count(), 0);
//    QVERIFY(m_storage->isInit() == false);
//    QCOMPARE(m_storage->version().data(), TEST_INVALID_NUMBER);
}

void Test_DB::testCreateWithInvalidVersionNegative()
{
//    m_storage = new Storage();
//    m_spies.append(new QSignalSpy(m_storage, SIGNAL(fireOnError(QString))));
//    m_spies.append(new QSignalSpy(m_storage, SIGNAL(fireOnCreate(QString,qint32))));
//    m_spies.append(new QSignalSpy(m_storage, SIGNAL(fireOnUpgrade(QString,qint32,qint32))));

//    QVERIFY(m_storage->isInit() == false);
//    QVERIFY((qint32)m_storage->version().result().type() != (qint32)QSqlError::NoError);
//    QCOMPARE(m_storage->version().data(), TEST_INVALID_NUMBER);

//    storage::STORAGE_CURR_VER = STORAGE_V1;
//    m_storage->init(m_dbName);
//    // Wait some time for initialization
//    QTest::qWait(200);

//    QCOMPARE(m_spies.at(SS_OnError)->count(), 1);
//    QCOMPARE(m_spies.at(SS_OnCreate)->count(), 0);
//    QCOMPARE(m_spies.at(SS_OnUpgrade)->count(), 0);
//    QVERIFY(m_storage->isInit() == false);
//    QCOMPARE(m_storage->version().data(), TEST_INVALID_NUMBER);
}

void Test_DB::testCreateWithCorrectV1()
{
//    m_storage = new Storage();
//    m_spies.append(new QSignalSpy(m_storage, SIGNAL(fireOnError(QString))));
//    m_spies.append(new QSignalSpy(m_storage, SIGNAL(fireOnCreate(QString,qint32))));
//    m_spies.append(new QSignalSpy(m_storage, SIGNAL(fireOnUpgrade(QString,qint32,qint32))));
//    // Before init
//    QVERIFY(m_storage->isInit() == false);
//    QCOMPARE(m_storage->version().data(), TEST_INVALID_NUMBER);
//    // Init

//    storage::STORAGE_CURR_VER = STORAGE_V1;
//    m_storage->init(m_dbName);
//    // Wait some time for initialization
//    QTest::qWait(200);

//    // One fires no error
//    QCOMPARE(m_spies.at(SS_OnError)->count(), 0);
//    // One fires just one onCreate
//    QCOMPARE(m_spies.at(SS_OnCreate)->count(), 1);
//    // One fires no upgrade
//    QCOMPARE(m_spies.at(SS_OnUpgrade)->count(), 0);
//    QVERIFY(m_storage->isInit() == true);
//    SqlIntResult_t v = m_storage->version();
//    QCOMPARE(v.data(), TEST_DB_V1);
//    QVERIFY2(v.data() == TEST_DB_V1, v.result().text().toLatin1());

//    const QString tblNoteName = "tbl_note";
//    bool exist = m_storage->contains(tblNoteName);
//    QVERIFY2(exist, tr("table '%1' exists").arg(tblNoteName).toLatin1());
}

void Test_DB::testCreateWithCorrectV2()
{
//    m_storage = new Storage();
//    m_spies.append(new QSignalSpy(m_storage, SIGNAL(fireOnError(QString))));
//    m_spies.append(new QSignalSpy(m_storage, SIGNAL(fireOnCreate(QString,qint32))));
//    m_spies.append(new QSignalSpy(m_storage, SIGNAL(fireOnUpgrade(QString,qint32,qint32))));
//    // Before init
//    QVERIFY(m_storage->isInit() == false);
//    QCOMPARE(m_storage->version().data(), TEST_INVALID_NUMBER);
//    // Init
//    storage::STORAGE_CURR_VER = STORAGE_V2;
//    m_storage->init(m_dbName);
//    // Wait some time for initialization
//    QTest::qWait(200);

//    // One fires no error
//    QCOMPARE(m_spies.at(SS_OnError)->count(), 0);
//    // One fires just one onCreate
//    QCOMPARE(m_spies.at(SS_OnCreate)->count(), 1);
//    // One fires no upgrade
//    QCOMPARE(m_spies.at(SS_OnUpgrade)->count(), 0);
//    QVERIFY(m_storage->isInit() == true);
//    SqlIntResult_t v = m_storage->version();
//    QCOMPARE(v.data(), TEST_DB_V2);
//    QVERIFY2(v.data() == TEST_DB_V2, v.result().text().toLatin1());
}

