#include <QSignalSpy>

#include "test_records.h"
#include "test_common_defs.h"

TestRecords::TestRecords(QObject *parent) : QObject(parent)
{

}

TestRecords::~TestRecords()
{

}

void TestRecords::initTestCase()
{
    m_waitTime = 20;
    m_dbName = test::TEST_DB_NAME;
    QVERIFY(test::deleteDB(m_dbName));
    QVERIFY2(m_storage==nullptr, "m_storage == nullptr");

    // 1. Creates DB
    m_storage = new SqliteStorage();
    QSignalSpy spy(m_storage, SIGNAL(fireInitProgress(ProgressInfo)));
    QVERIFY2(m_storage->isInit()==false, "Check isInit before init()");
    m_storage->init(m_dbName);
    QTest::qWait(m_waitTime);
    QList<QVariant> arguments = spy.takeLast(); // take the last signal
    ProgressInfo pi = arguments.at(0).value<ProgressInfo>();
    QVERIFY2(pi.status == ProgressInfo::TPS_Success, pi.message.toLatin1());
}

void TestRecords::cleanupTestCase()
{
    delete m_storage;
    m_storage = nullptr;
}

void TestRecords::init()
{
    //  1. Remove recs
    //  2. Check it
    {
        QSignalSpy spy(m_storage, SIGNAL(fireTaskProgress(ProgressInfo,QVariant)));
        const qint64 id = QDateTime::currentMSecsSinceEpoch();
        m_storage->clearNotes(id);
        QTest::qWait(m_waitTime);
        QList<QVariant> arguments = spy.takeLast(); // take the last signal
        ProgressInfo pi = arguments.at(0).value<ProgressInfo>();
        QVERIFY2(pi.status == ProgressInfo::TPS_Success, pi.message.toLatin1());
        QCOMPARE(pi.id, id);
    }
    {
        QSignalSpy spy(m_storage, SIGNAL(fireTaskProgress(ProgressInfo,QVariant)));
        const qint64 id = QDateTime::currentMSecsSinceEpoch();
        m_storage->fetchNotes(id);
        QTest::qWait(m_waitTime);
        QList<QVariant> arguments = spy.takeLast(); // take the last signal
        ProgressInfo pi = arguments.at(0).value<ProgressInfo>();
        QVERIFY2(pi.status == ProgressInfo::TPS_Success, pi.message.toLatin1());
        QCOMPARE(pi.id, id);

        QVariant v = arguments.at(1);
        QVERIFY(v.canConvert< QSharedPointer<QObject> >());
        QSharedPointer<QObject> sp = v.value< QSharedPointer<QObject> >();
        QVERIFY(sp);
        QSharedPointer<NoteList> spnl = qSharedPointerDynamicCast<NoteList>(sp);
        QVERIFY(spnl);
        QCOMPARE(spnl->data().count(), 0);
    }
}

void TestRecords::cleanup()
{
    delete m_notes;
    m_notes = nullptr;
}

void TestRecords::testAddNote()
{
    //  1. Add 2 notes
    //  2. Check
    {
        const qint32 cnt = 20;
        QSignalSpy spy(m_storage, SIGNAL(fireTaskProgress(ProgressInfo,QVariant)));
        NoteList *nl = new NoteList;
        for(int i=0; i < cnt; ++i){
            Note *n = new Note();
            n->setAuthorId(i%3);
            nl->data().append(n);
        }

        const qint64 id = QDateTime::currentMSecsSinceEpoch();
        m_storage->addNotes(id, QSharedPointer<NoteList>(nl));
        QTest::qWait(m_waitTime);
        QList<QVariant> arguments = spy.takeLast(); // take the last signal
        ProgressInfo pi = arguments.at(0).value<ProgressInfo>();
        QVERIFY2(pi.status == ProgressInfo::TPS_Success, pi.message.toLatin1());
        QCOMPARE(pi.id, id);

        QVariant v = arguments.at(1);
        QVERIFY(v.canConvert< QSharedPointer<QObject> >());
        QSharedPointer<QObject> sp = v.value< QSharedPointer<QObject> >();
        QVERIFY(sp);
        QSharedPointer<NoteList> spnl = qSharedPointerDynamicCast<NoteList>(sp);
        QVERIFY(spnl);
        QCOMPARE(spnl->data().count(), cnt);
    }
}
