#include <QSignalSpy>
#include <QList>
#include <QVariant>

#include "test_records.h"
#include "test_common_defs.h"

TestRecords::TestRecords(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<nq::ProgressInfo>();
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
    QSignalSpy spy(m_storage, SIGNAL(fireInitProgress(nq::ProgressInfo)));
    QVERIFY2(m_storage->isInit()==false, "Check isInit before init()");
    m_storage->init(m_dbName);
    QTest::qWait(m_waitTime);
    QList<QVariant> arguments = spy.takeLast(); // take the last signal
    nq::ProgressInfo pi = arguments.at(0).value<nq::ProgressInfo>();
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
        QSignalSpy spy(m_storage, SIGNAL(fireTaskProgress(nq::ProgressInfo,QVariant)));
        const qint64 id = QDateTime::currentMSecsSinceEpoch();
        m_storage->clearNotes(id);
        QTest::qWait(m_waitTime);
        QList<QVariant> arguments = spy.takeLast(); // take the last signal
        ProgressInfo pi = arguments.at(0).value<ProgressInfo>();
        QVERIFY2(pi.status == ProgressInfo::TPS_Success, pi.message.toLatin1());
        QCOMPARE(pi.id, id);
    }
    {
        QSignalSpy spy(m_storage, SIGNAL(fireTaskProgress(nq::ProgressInfo,QVariant)));
        const qint64 id = QDateTime::currentMSecsSinceEpoch();
        m_storage->fetchNotes(id);
        QTest::qWait(m_waitTime);
        QList<QVariant> arguments = spy.takeLast(); // take the last signal
        ProgressInfo pi = arguments.at(0).value<ProgressInfo>();
        QVERIFY2(pi.status == ProgressInfo::TPS_Success, pi.message.toLatin1());
        QCOMPARE(pi.id, id);

        QVariant v = arguments.at(1);
        QVERIFY(v.canConvert< QSharedPointer<Notes> >());
        QSharedPointer<Notes> spnl = v.value< QSharedPointer<Notes> >();
//        QVERIFY(sp);
//        QSharedPointer<NoteModel> spnl = qSharedPointerDynamicCast<NoteModel>(sp);
        QVERIFY(spnl);
        QCOMPARE(spnl->items.count(), 0);
    }
}

void TestRecords::cleanup()
{
    delete m_notes;
    m_notes = nullptr;
}

void TestRecords::testAddNote()
{
//    //  1. Add 2 notes
//    //  2. Check
//    {
//        const qint32 cnt = 20;
//        QSignalSpy spy(m_storage, SIGNAL(fireTaskProgress(nq::ProgressInfo,QVariant)));
//        NoteModel *nl = new NoteModel;
//        for(int i=0; i < cnt; ++i){
//            Note *n = new Note();
//            n->setAuthorId(i%3);
//            nl->items().append(n);
//        }

//        const qint64 id = QDateTime::currentMSecsSinceEpoch();
//        m_storage->addNotes(id, QSharedPointer<NoteModel>(nTableTagsl));
//        QTest::qWait(m_waitTime);
//        QList<QVariant> arguments = spy.takeLast(); // take the last signal
//        ProgressInfo pi = arguments.at(0).value<ProgressInfo>();
//        QVERIFY2(pi.status == ProgressInfo::TPS_Success, pi.message.toLatin1());
//        QCOMPARE(pi.id, id);

//        QVariant v = arguments.at(1);
//        QVERIFY(v.canConvert< QSharedPointer<QObject> >());
//        QSharedPointer<QObject> sp = v.value< QSharedPointer<QObject> >();
//        QVERIFY(sp);
//        QSharedPointer<NoteModel> spnl = qSharedPointerDynamicCast<NoteModel>(sp);
//        QVERIFY(spnl);
//        QCOMPARE(spnl->items().count(), cnt);
//    }
}

void TestRecords::testAddNote3()
{
    {
        // Just test for memory control.
        model::Notes *mn1 = new Notes;
        mn1->items.append(new Note);
        model::Notes *mn2 = mn1;
        QSharedPointer<model::Notes> sp1(mn1);
        QVariant v1 = QVariant::fromValue(sp1);
        QSharedPointer<model::Notes> sp2 = v1.value<QSharedPointer<model::Notes> >();
        QSharedPointer<model::Notes> sp3 = sp2;
        mn1 = sp1.data();
        mn2 = sp2.data();
        bool b1 = sp1.isNull();
        bool b2 = sp2.isNull();
        bool b3 = sp2.isNull();
        sp3->items.append(new Note);
        Q_UNUSED(mn2);
        Q_UNUSED(b1);
        Q_UNUSED(b2);
        Q_UNUSED(b3);

    }


    //  1. Add 'cnt' notes
    //  2. Check
    {
        const qint32 cnt = 50;
        QSignalSpy spy(m_storage, SIGNAL(fireTaskProgress(nq::ProgressInfo,QVariant)));
        Notes *notes = new Notes;
        for(int i=0; i < cnt; ++i){
            Note *n = new Note();
            n->setAuthorId(i%3);
            notes->items.append(n);
        }

        const qint64 id = QDateTime::currentMSecsSinceEpoch();
        m_storage->addNotes3(id, QSharedPointer<Notes>(notes));
        QTest::qWait(m_waitTime);
        QList<QVariant> arguments = spy.takeLast(); // take the last signal
        ProgressInfo pi = arguments.at(0).value<ProgressInfo>();
        QVERIFY2(pi.status == ProgressInfo::TPS_Success, pi.message.toLatin1());
        QCOMPARE(pi.id, id);

        QVariant v = arguments.at(1);
        QVERIFY(v.canConvert< QSharedPointer<Notes> >());
        QSharedPointer<Notes> sp = v.value< QSharedPointer<Notes> >();
        QVERIFY(sp);
        QCOMPARE(sp->items.count(), cnt);
    }
}

