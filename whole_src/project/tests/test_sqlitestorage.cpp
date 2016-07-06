#include <QSqlQuery>
#include <QSqlDriver>
#include <QString>

#include "notemodel.h"
#include "authormodel.h"

#include "test_sqlitestorage.h"
#include "test_common_defs.h"
#include "sqlite_storage_v1.h"
#include "sqlite_storage_v2.h"
#include "sqlitestorage_elements.h"

using namespace storage;

//TestSqliteStorage::TestSqliteStorage(QObject *parent) : QObject(parent)
//{

//}

//TestSqliteStorage::~TestSqliteStorage()
//{

//}

void TestSqliteStorage::initTestCase()
{
//    QSKIP("Temp skip");

    qRegisterMetaType<nq::ProgressInfo>();
    qRegisterMetaType<model::NoteModel*>();
    //m_dbName = "TestSqliteStorage.db";
    m_initTime = 20;
    m_dbName = test::TEST_DB_NAME;
}

void TestSqliteStorage::cleanupTestCase()
{
//    STORAGE_CURR_VER = m_origVersion;
}

void TestSqliteStorage::init()
{
    QVERIFY(test::deleteDB(m_dbName));
    QVERIFY2(m_storage==nullptr, "m_storage == nullptr");
}

void TestSqliteStorage::cleanup()
{
//    storage::CURR_VERSION = storage::V1;
    // Deletes physical file;
    // Deletes storage (and QSqlDatabase)
    m_storage = nullptr;
}

void TestSqliteStorage::testCreateV3()
{
    // 1.
    m_storage = &SqliteStorage::instance();
    QSignalSpy spy(m_storage, SIGNAL(fireInitProgress(nq::ProgressInfo)));
    //m_spies.append(new QSignalSpy(m_storage, SIGNAL(fireInitProgress(ProgressInfo))));

    // Version is 1
    m_storage->init(m_dbName);
    // Wait some time for initialization
    QTest::qWait(m_initTime);
    QList<QVariant> arguments = spy.takeLast(); // take the last signal
    ProgressInfo pi = arguments.at(0).value<ProgressInfo>();
    QVERIFY2(pi.status == ProgressInfo::TPS_Success, pi.message.toLatin1());

    BoolVariantResult_t tablesRes = m_storage->tables();
    QVERIFY2(tablesRes.result() == true, tablesRes.data().toString().toLatin1());
    QStringList tables = tablesRes.data().toStringList();
    QCOMPARE(tables.count(), 6);
    QVERIFY(tables.contains(storage::sqlite::TableCfg::TBL_NAME));
    QVERIFY(tables.contains(storage::sqlite::TableNote::TBL_NAME));
    QVERIFY(tables.contains(storage::sqlite::TableAuthor::TBL_NAME));
    QVERIFY(tables.contains(storage::sqlite::TableTags::TBL_NAME));
    QVERIFY(tables.contains(storage::sqlite::TableTagsNotes::TBL_NAME));

//    BoolVariantResult_t v1Created = createV1();
//    QVERIFY2(v1Created.result(), v1Created.data().toString().toLatin1());
//    BoolVariantResult_t v2Created = createV2();
//    QVERIFY2(v2Created.result(), v2Created.data().toString().toLatin1());
}

void TestSqliteStorage::testUpgradeV1V3()
{
    //  1. Create V1 with data
    //  2. Start init()
    //  3. Storage must define that DB has version 1 and start upgrading to V3
    BoolVariantResult_t v1Created = createV1();
    QVERIFY2(v1Created.result(), v1Created.data().toString().toLatin1());

    m_storage = &SqliteStorage::instance();
    QSignalSpy spy(m_storage, SIGNAL(fireInitProgress(nq::ProgressInfo)));
    m_storage->init(m_dbName);
    // Wait some time for initialization
    QTest::qWait(m_initTime);
    QList<QVariant> arguments = spy.takeLast(); // take the last signal
    ProgressInfo pi = arguments.at(0).value<ProgressInfo>();
    QVERIFY2(pi.status == ProgressInfo::TPS_Success, pi.message.toLatin1());

    BoolVariantResult_t tablesRes = m_storage->tables();
    QVERIFY2(tablesRes.result() == true, tablesRes.data().toString().toLatin1());
    QStringList tables = tablesRes.data().toStringList();
    QCOMPARE(tables.count(), 6);
    QVERIFY(tables.contains(storage::sqlite::TableCfg::TBL_NAME));
    QVERIFY(tables.contains(storage::sqlite::TableNote::TBL_NAME));
    QVERIFY(tables.contains(storage::sqlite::TableAuthor::TBL_NAME));
    QVERIFY(tables.contains(storage::sqlite::TableTags::TBL_NAME));
    QVERIFY(tables.contains(storage::sqlite::TableTagsNotes::TBL_NAME));
    QVERIFY(tables.contains(storage::sqlite::TableTmpFromSrv::TBL_NAME));

    // Check al notes in DB (they mast be saved)
//    QList<QVariant> l;
//    QVariant vn(QVariant::fromValue(new model::Note()));
//    l.append(vn);
//    QVariant v(l);
//    BoolVariantResult_t bvr(false, l);
//    return {false, v};

    {
        QSignalSpy spyTask(m_storage, SIGNAL(fireTaskProgress(nq::ProgressInfo,QVariant)));
        const qint64 id = QDateTime::currentMSecsSinceEpoch();
        //
        m_storage->fetchNotes(id, UuidVector_t());
        QTest::qWait(m_initTime);
        QList<QVariant> arguments = spyTask.takeLast(); // take the last signal
        ProgressInfo pi = arguments.at(0).value<ProgressInfo>();
        QVERIFY2(pi.status == ProgressInfo::TPS_Success, pi.message.toLatin1());
        QCOMPARE(pi.id, id);
        QSharedPointer<Notes> spnl = arguments.at(1).value< QSharedPointer<Notes> >();
        QVERIFY(spnl);
//        QSharedPointer<NoteModel> spnl = qSharedPointerDynamicCast<NoteModel>(sp);
//        QVERIFY(spnl);
        QCOMPARE(spnl->items.count(), 3);
    }
}

void TestSqliteStorage::testUpgradeV2V3()
{
    //  1. Create V2 with data
    //  2. Start init()
    //  3. Storage must define that DB has version 1 and start upgrading to V3
    BoolVariantResult_t v2Created = createV2();
    QVERIFY2(v2Created.result(), v2Created.data().toString().toLatin1());

    m_storage = &SqliteStorage::instance();
    QSignalSpy spy(m_storage, SIGNAL(fireInitProgress(nq::ProgressInfo)));
    m_storage->init(m_dbName);
    // Wait some time for initialization
    QTest::qWait(m_initTime);
    QList<QVariant> arguments = spy.takeLast(); // take the last signal
    ProgressInfo pi = arguments.at(0).value<ProgressInfo>();
    QVERIFY2(pi.status == ProgressInfo::TPS_Success, pi.message.toLatin1());

    BoolVariantResult_t tablesRes = m_storage->tables();
    QVERIFY2(tablesRes.result() == true, tablesRes.data().toString().toLatin1());
    QStringList tables = tablesRes.data().toStringList();
    QCOMPARE(tables.count(), 6);
    QVERIFY(tables.contains(storage::sqlite::TableCfg::TBL_NAME));
    QVERIFY(tables.contains(storage::sqlite::TableNote::TBL_NAME));
    QVERIFY(tables.contains(storage::sqlite::TableAuthor::TBL_NAME));
    QVERIFY(tables.contains(storage::sqlite::TableTags::TBL_NAME));
    QVERIFY(tables.contains(storage::sqlite::TableTagsNotes::TBL_NAME));
    QVERIFY(tables.contains(storage::sqlite::TableTmpFromSrv::TBL_NAME));

    {
        QSignalSpy spyTask(m_storage, SIGNAL(fireTaskProgress(nq::ProgressInfo,QVariant)));
        const qint64 id = QDateTime::currentMSecsSinceEpoch();
        m_storage->fetchNotes(id, UuidVector_t());
        QTest::qWait(m_initTime);
        QList<QVariant> arguments = spyTask.takeLast(); // take the last signal
        ProgressInfo pi = arguments.at(0).value<ProgressInfo>();
        QVERIFY2(pi.status == ProgressInfo::TPS_Success, pi.message.toLatin1());
        QCOMPARE(pi.id, id);
        QSharedPointer<Notes> spnl = arguments.at(1).value< QSharedPointer<Notes> >();
        QVERIFY(spnl);
//        QSharedPointer<NoteModel> spnl = qSharedPointerDynamicCast<NoteModel>(sp);
//        QVERIFY(spnl);
        QCOMPARE(spnl->items.count(), 3);
    }
    {
        QSignalSpy spy(m_storage, SIGNAL(fireTaskProgress(nq::ProgressInfo,QVariant)));
        const qint64 id = QDateTime::currentMSecsSinceEpoch();
        m_storage->fetchAuthors(id);
        QTest::qWait(m_initTime);
        QList<QVariant> arguments = spy.takeLast(); // take the last signal
        ProgressInfo pi = arguments.at(0).value<ProgressInfo>();
        QVERIFY2(pi.status == ProgressInfo::TPS_Success, pi.message.toLatin1());
        QCOMPARE(pi.id, id);
        QSharedPointer<QObject> sp = arguments.at(1).value< QSharedPointer<QObject> >();
        QVERIFY(sp);
        QSharedPointer<AuthorModel> spnl = qSharedPointerDynamicCast<AuthorModel>(sp);
        QVERIFY(spnl);
        QCOMPARE(spnl->data().count(), 1);
    }
}

BoolVariantResult_t TestSqliteStorage::createV1()
{
    QSqlDatabase db = QSqlDatabase::database(QStringLiteral("v1"));
    if(!db.isOpen()) {
        db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), QStringLiteral("v1"));
        db.setDatabaseName(m_dbName);
        if (!db.open()){
            return {false, db.lastError().text()};
        }
    }

    // if ok
    using namespace storage::sqlite;
    {
        // 'Config' table and data
        QSqlQuery q(db);
        if (!q.exec(v1::TableCfg::QRY_CREATE)){
            return {false, q.lastError().text()};
        }

        const QString QRY_INSERT(QStringLiteral("insert into %1(%2, %3) values(?, ?)")
                                 .arg(v1::TableCfg::TBL_NAME).arg(v1::TableCfg::FLD_THE_KEY)
                                 .arg(v1::TableCfg::FLD_THE_VALUE));
        if (!q.prepare(QRY_INSERT)){
            return {false, q.lastError().text()};
        }
        q.addBindValue("version");
        q.addBindValue(1);
        if (!q.exec()){
            return {false, q.lastError().text()};
        }
    }
    {
        // 'Note' table and data
        QSqlQuery q(db);
        if (!q.exec(v1::TableNote::QRY_CREATE)){
            return {false, q.lastError().text()};
        }

        const QString QRY_INSERT(QStringLiteral("insert into %1(%2) values(?)")
                                 .arg(v1::TableNote::TBL_NAME).arg(v1::TableNote::FLD_THE_TEXT));
        if (!q.prepare(QRY_INSERT)){
            return {false, q.lastError().text()};
        }
        q.addBindValue("note1");
        if (!q.exec()){
            return {false, q.lastError().text()};
        }
        q.addBindValue("note2");
        if (!q.exec()){
            return {false, q.lastError().text()};
        }
        q.addBindValue("note3");
        if (!q.exec()){
            return {false, q.lastError().text()};
        }
    }

    return {true, Q_FUNC_INFO};
}

BoolVariantResult_t TestSqliteStorage::createV2()
{
    QSqlDatabase db = QSqlDatabase::database(QStringLiteral("v2"));
    if(!db.isOpen()) {
        db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), QStringLiteral("v2"));
        db.setDatabaseName(m_dbName);
        if (!db.open()){
            return {false, db.lastError().text()};
        }
    }

    bool f = db.driver()->hasFeature(QSqlDriver::LastInsertId);
    if(!f) {
        return {false, "No feature 'QSqlDriver::LastInsertId'"};
    }

    // if ok
    using namespace storage::sqlite;
    {
        // 'Config' table and data
        QSqlQuery q(db);
        if (!q.exec(v2::TableCfg::QRY_CREATE)){
            return {false, q.lastError().text()};
        }

        const QString QRY_INSERT(QStringLiteral("insert into %1(%2, %3, %4) values(?, ?, ?)")
                                 .arg(v2::TableCfg::TBL_NAME).arg(v2::TableCfg::FLD_KEY)
                                 .arg(v2::TableCfg::FLD_VALUE).arg(v2::TableCfg::FLD_DESCR));
        if (!q.prepare(QRY_INSERT)){
            return {false, q.lastError().text()};
        }
        q.addBindValue("version");
        q.addBindValue(2);
        q.addBindValue("DB current version");
        if (!q.exec()){
            return {false, q.lastError().text()};
        }
    }

    QVariant authId;
    {
        // 'Author' table and data
        QSqlQuery q(db);
        if (!q.exec(v2::TableAuthor::QRY_CREATE)){
            return {false, q.lastError().text()};
        }

        const QString QRY_INSERT(QStringLiteral("insert into %1(%2) values(?)")
                                 .arg(v2::TableAuthor::TBL_NAME).arg(v2::TableAuthor::FLD_TITLE));
        if (!q.prepare(QRY_INSERT)){
            return {false, q.lastError().text()};
        }
        q.addBindValue("author1");
        authId = q.exec();
        if (!authId.isValid()){
            return {false, q.lastError().text()};
        }
    }

    {
        // 'Note' table and data
        QSqlQuery q(db);
        if (!q.exec(v2::TableNote::QRY_CREATE)){
            return {false, q.lastError().text()};
        }

        const QString QRY_INSERT(QStringLiteral("insert into %1(%2, %3) values(?, ?)")
                                 .arg(v2::TableNote::TBL_NAME).arg(v2::TableNote::FLD_TEXT)
                                 .arg(v2::TableNote::FLD_FK_AUTHOR));
        if (!q.prepare(QRY_INSERT)){
            return {false, q.lastError().text()};
        }
        q.addBindValue("note1");
        q.addBindValue(authId.toInt());
        if (!q.exec()){
            return {false, q.lastError().text()};
        }
        q.addBindValue("note2");
        q.addBindValue(authId.toInt());
        if (!q.exec()){
            return {false, q.lastError().text()};
        }
        q.addBindValue("note3");
        q.addBindValue(authId.toInt());
        if (!q.exec()){
            return {false, q.lastError().text()};
        }
    }

    return {true, Q_FUNC_INFO};
}

