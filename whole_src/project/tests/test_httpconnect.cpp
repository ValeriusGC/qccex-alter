#include <QSignalSpy>
#include <QMetaType>

#include "test_httpconnect.h"
#include "note.h"
#include "shared_utils.h"
#include "httpclient.h"
#include "request.h"
#include "reply.h"
#include "storage_globals.h"

using namespace model;
using namespace storage;

const qint32 HTTP_TIMEOUT_MSEC = 1500;

TestHttpConnect::TestHttpConnect(QObject *parent) : QObject(parent)
{

}

TestHttpConnect::~TestHttpConnect()
{

}

void TestHttpConnect::initTestCase()
{
//    QSKIP("Temp skip");
    m_srvAddress = "http://gdetotut.com/sn/input.php";

    // 16/06/23 With Local works good
///    m_srvAddress = "http://localhost/snserver/input.php";
    m_account = "frostyland@yandex.ru";
    m_psw = "123456";
}

void TestHttpConnect::cleanupTestCase()
{

}

void TestHttpConnect::init()
{

}

void TestHttpConnect::cleanup()
{

}

void TestHttpConnect::testRequest()
{
    using storage::Request;

    // 1.
    {
        QScopedPointer<Request> request(new Request("", "", 10, 20));
        QVERIFY2(request->state().result() == Request::CIDError, request->state().data().toLatin1());
    }
    {
        QScopedPointer<Request> request(new Request("cid", "", 10, 20));
        QVERIFY2(request->state().result() == Request::PswError, request->state().data().toLatin1());
    }
    {
        QScopedPointer<Request> request(new Request("cid", "pwd", 10, 20));
        QVERIFY2(request->state().result() == Request::NoError, request->toJson().toLatin1());
    }

    // 2.
    using storage::Reply;
    {
        QScopedPointer<Request> request(new Request("cid", "psw", 10, 20));
        QScopedPointer<Reply> reply(new Reply(request->toJson()));
        BoolResult_t check = reply->matches(*request.data());
        QVERIFY2(check.result() == true, check.data().toLatin1());
    }

}

void TestHttpConnect::testJson()
{
    {
        QScopedPointer<Note> note1 (new Note(Utils::createBase64Uuid()));
        QJsonObject jo = note1->toJsonObject();
        QScopedPointer<Note> note2 (Note::fromJsonObject(jo));
        QVERIFY2(!jo.isEmpty(), "QJsonObject is empty");
        QVERIFY2(note1->equalTo(note2.data()), QJsonDocument(note2->toJsonObject()).toJson());
    }

}

void TestHttpConnect::testJsonParser()
{
    {
        QString candidate = "{}";
        Reply reply(candidate);
        QVERIFY2(reply.state().result() != Reply::NoError, reply.state().data().toLatin1());
        QVERIFY2(reply.state().result() == Reply::BadHeader, reply.state().data().toLatin1());
    }

    {
        // Valid with all parts
        // hdr:{ver, cid, qry_type, qry_ord}, body:{} ...
        QString str = QString("{\"%1\":{ \"%3\":%4, \"%5\":\"%6\", \"%7\":%8, \"%9\":%10, \"%11\":%12 }, "
                                    "\"%2\":{}  }");
        {
            // Valid with all parts
            QString candidate = str.arg(Reply::IDS_HEADER).arg(Reply::IDS_BODY)
                    .arg(Reply::IDS_VER).arg(1)
                    .arg(Reply::IDS_CID).arg("client_uuid")
                    .arg(Reply::IDS_TYPE).arg(1)
                    .arg(Reply::IDS_ORD).arg(1)
                    .arg(Reply::IDS_RESULT).arg(10)
                    ;
            Reply reply(candidate);
            QVERIFY2(reply.state().result() == Reply::NoError, reply.state().data().toLatin1());

        }

        {
            // With invalid version
            QString candidate = str.arg(Reply::IDS_HEADER).arg(Reply::IDS_BODY)
                    .arg(Reply::IDS_VER).arg("\"no_int\"")
                    .arg(Reply::IDS_CID).arg("client_uuid")
                    .arg(Reply::IDS_TYPE).arg(1)
                    .arg(Reply::IDS_ORD).arg(1)
                    .arg(Reply::IDS_RESULT).arg(10)
                    ;
            Reply reply(candidate);
            QVERIFY2(reply.state().result() == Reply::BadHeader, reply.state().data().toLatin1());
        }
        {
            // With invalid CID
            QString candidate = str.arg(Reply::IDS_HEADER).arg(Reply::IDS_BODY)
                    .arg(Reply::IDS_VER).arg(1)
                    .arg(Reply::IDS_CID).arg("")
                    .arg(Reply::IDS_TYPE).arg(1)
                    .arg(Reply::IDS_ORD).arg(1)
                    .arg(Reply::IDS_RESULT).arg(10)
                    ;
            Reply reply(candidate);
            QVERIFY2(reply.state().result() == Reply::BadHeader, reply.state().data().toLatin1());
        }
        {
            // With invalid TYPE
            QString candidate = str.arg(Reply::IDS_HEADER).arg(Reply::IDS_BODY)
                    .arg(Reply::IDS_VER).arg(1)
                    .arg(Reply::IDS_CID).arg("client_uuid")
                    .arg(Reply::IDS_TYPE).arg("\"no_int\"")
                    .arg(Reply::IDS_ORD).arg(1)
                    .arg(Reply::IDS_RESULT).arg(10)
                    ;
            Reply reply(candidate);
            QVERIFY2(reply.state().result() == Reply::BadHeader, reply.state().data().toLatin1());
        }
        {
            // With invalid ORDINAL
            QString candidate = str.arg(Reply::IDS_HEADER).arg(Reply::IDS_BODY)
                    .arg(Reply::IDS_VER).arg(1)
                    .arg(Reply::IDS_CID).arg("client_uuid")
                    .arg(Reply::IDS_TYPE).arg(1)
                    .arg(Reply::IDS_ORD).arg("\"no_int\"")
                    .arg(Reply::IDS_RESULT).arg(10)
                    ;
            Reply reply(candidate);
            QVERIFY2(reply.state().result() == Reply::BadHeader, reply.state().data().toLatin1());
        }
        {
            // With invalid RESULT
            QString candidate = str.arg(Reply::IDS_HEADER).arg(Reply::IDS_BODY)
                    .arg(Reply::IDS_VER).arg(1)
                    .arg(Reply::IDS_CID).arg("client_uuid")
                    .arg(Reply::IDS_TYPE).arg(1)
                    .arg(Reply::IDS_ORD).arg("\"no_int\"")
                    .arg(Reply::IDS_RESULT).arg("pass_111")
                    ;
            Reply reply(candidate);
            QVERIFY2(reply.state().result() == Reply::BadHeader, reply.state().data().toLatin1());
        }

    }

}

void TestHttpConnect::testHttpHandshake()
{
//    QSKIP("Temp skip");

    qRegisterMetaType< QSharedPointer<Reply> >("QSharedPointer<Reply>");
    {
        // For TEST sake we send correct string from project 'sndesktop' (Netbeans)
//        QString TEST_REQUEST = "{\"hdr\":{\"ver\":2,\"cid\":\"mycid\",\"psw\":\"111\","
//                "\"type\":3101,\"ord\":3344,\"dbg\":1},"
//                "\"body\":{}"
//                "}";
        QScopedPointer<Request> request(new Request("my+cid", "mypsq", 3101, 10, 1));

        QVERIFY2(request->state().result() == Request::NoError, request->state().data().toLatin1());

        QScopedPointer<HTTPClient> httpClient(new HTTPClient);
        QSignalSpy spy(httpClient.data(), SIGNAL(fireReply(QSharedPointer<Reply>)));
        httpClient->startRequest(QUrl(m_srvAddress), *request.data());
        QTest::qWait(HTTP_TIMEOUT_MSEC);
        QList<QVariant> arguments = spy.takeLast(); // take the last signal

        QSharedPointer<Reply> jpReply = arguments.at(0).value<QSharedPointer<Reply>>();
        QVERIFY2(jpReply->state().result() == Reply::NoError, jpReply->state().data().toLatin1());

    }

    {
        // Now send Handshake query:
        // Awaits some server data back.
        // Prepare
        const qint32 ver = 5;
        const qint32 type = 10;
        const qint32 ord = 30;
        const qint32 res = 44;
        const QString cid = "my_cid";
        QString str = QString("{ \"%1\":{ \"%3\":%4, \"%5\":\"%6\", \"%7\":%8, \"%9\":%10, \"%11\":%12 }, \"%2\":{}  }");
        QString candidate = str.arg(Reply::IDS_HEADER).arg(Reply::IDS_BODY)
                .arg(Reply::IDS_VER).arg(ver)
                .arg(Reply::IDS_CID).arg(cid)
                .arg(Reply::IDS_TYPE).arg(type)
                .arg(Reply::IDS_ORD).arg(ord)
                .arg(Reply::IDS_RESULT).arg(res)
                ;
        Reply jp(candidate);
        QVERIFY2(jp.state().result() == Reply::NoError, jp.state().data().toLatin1());
    }
}

void TestHttpConnect::testQueryForNotes()
{
//    QSKIP("Temp skip");
    qint32 ord = 1;

    {
        // 1. Delete notes
        QScopedPointer<Request> request(new Request("fr@q.co", "psw01", Const::REQ_TYPE_DEL_NOTES, ord, 1));
        QVERIFY2(request->state().result() == Request::NoError, request->state().data().toLatin1());
        // Create http, send request
        QScopedPointer<HTTPClient> httpClient(new HTTPClient);
        QSignalSpy spy(httpClient.data(), SIGNAL(fireReply(QSharedPointer<Reply>)));
        httpClient->startRequest(QUrl(m_srvAddress), *request.data());
        QTest::qWait(HTTP_TIMEOUT_MSEC);
        // Get reply, check attributes
        QList<QVariant> arguments = spy.takeLast(); // take the last signal
        QSharedPointer<Reply> reply = arguments.at(0).value<QSharedPointer<Reply>>();
        // Check correct JSON
        QVERIFY2(reply->state().result() == Reply::NoError, reply->state().data().toLatin1());
        // Check correct fields
        BoolResult_t res = reply->matches(*request.data());
        QVERIFY2(res.result(), res.data().toLatin1());
        QCOMPARE(reply->hdr()[Reply::IDS_RESULT].toInt(Const::INVALID), Const::SUCCESS);
    }

    {
        // 2. Get count (0)

        //-----------------------------------------------------------
        // Structure request
        //  HDR:
        //
        //  BODY:
        //-----------------------------------------------------------
        // Structure reply
        //  HDR:
        //
        //  BODY:
        //      {RESULT=count(int)}
        //-----------------------------------------------------------

        ord += 1;

        // Build request
        QScopedPointer<Request> request(new Request("fr@q.co", "psw01", Const::REQ_TYPE_COUNT_NOTES, ord, 1));
        QVERIFY2(request->state().result() == Request::NoError, request->state().data().toLatin1());

        QScopedPointer<HTTPClient> httpClient(new HTTPClient);
        QSignalSpy spy(httpClient.data(), SIGNAL(fireReply(QSharedPointer<Reply>)));
        httpClient->startRequest(QUrl(m_srvAddress), *request.data());
        QTest::qWait(HTTP_TIMEOUT_MSEC);
        QList<QVariant> arguments = spy.takeLast(); // take the last signal

        QSharedPointer<Reply> reply = arguments.at(0).value<QSharedPointer<Reply>>();
        // Check correct JSON
        QVERIFY2(reply->state().result() == Reply::NoError, reply->state().data().toLatin1());
        // Check correct fields
        BoolResult_t matches = reply->matches(*request.data());
        QVERIFY2(matches.result(), matches.data().toLatin1());
        QCOMPARE(reply->hdr()[Reply::IDS_RESULT].toInt(Const::INVALID), Const::SUCCESS);

        const qint32 cnt = reply->body()["RESULT"].toInt(Const::INVALID);
        QCOMPARE(cnt, 0);
    }


    QVector< QSharedPointer<Note> > noteVector;
    {
        // 3. Add pair of note
        ord += 1;

        //-----------------------------------------------------------
        // Structure request
        //  HDR:
        //
        //  BODY:
        //      {Const::BODY_SET_NOTES : [array of Note-objects]}
        //-----------------------------------------------------------
        // Structure reply
        //  HDR:
        //
        //  BODY:
        //      {[array of uuids for non-inserted Note-objects] or [null]}
        //-----------------------------------------------------------

        // Build request

        QScopedPointer<Request> request(new Request("fr@q.co", "psw01", Const::REQ_TYPE_ADD_NOTES, ord, 1));
        QVERIFY2(request->state().result() == Request::NoError, request->state().data().toLatin1());

        QJsonArray notes;
        for(int i=0; i<2; ++i) {
            const QString uuid = Utils::createBase64Uuid();
            QSharedPointer<Note> note(new Note(uuid));
            noteVector.append(note);
            note->setAsDel(1);
            note->setAuthorId(5);
            note->setText(QString("note_%1").arg(i));
            QJsonObject jsonNote = note->toHttpJsonObject();
            notes.append(jsonNote);
            request->insert(Const::BODY_SET_NOTES, notes);
        }

        QScopedPointer<HTTPClient> httpClient(new HTTPClient);
        QSignalSpy spy(httpClient.data(), SIGNAL(fireReply(QSharedPointer<Reply>)));
        httpClient->startRequest(QUrl(m_srvAddress), *request.data());
        QTest::qWait(HTTP_TIMEOUT_MSEC);
        QList<QVariant> arguments = spy.takeLast(); // take the last signal

        QSharedPointer<Reply> reply = arguments.at(0).value<QSharedPointer<Reply>>();
        // Check correct JSON
        QVERIFY2(reply->state().result() == Reply::NoError, reply->state().data().toLatin1());
        // Check correct fields
        BoolResult_t matches = reply->matches(*request.data());
        QVERIFY2(matches.result(), matches.data().toLatin1());
        QCOMPARE(reply->hdr()[Reply::IDS_RESULT].toInt(Const::INVALID), Const::SUCCESS);
    }

    {
        // 2. Get count (2)

        //-----------------------------------------------------------
        // Structure request
        //  HDR:
        //
        //  BODY:
        //-----------------------------------------------------------
        // Structure reply
        //  HDR:
        //
        //  BODY:
        //      {RESULT=count(int)}
        //-----------------------------------------------------------

        ord += 1;

        // Build request
        QScopedPointer<Request> request(new Request("fr@q.co", "psw01", Const::REQ_TYPE_COUNT_NOTES, ord, 1));
        QVERIFY2(request->state().result() == Request::NoError, request->state().data().toLatin1());

        QScopedPointer<HTTPClient> httpClient(new HTTPClient);
        QSignalSpy spy(httpClient.data(), SIGNAL(fireReply(QSharedPointer<Reply>)));
        httpClient->startRequest(QUrl(m_srvAddress), *request.data());
        QTest::qWait(HTTP_TIMEOUT_MSEC);
        QList<QVariant> arguments = spy.takeLast(); // take the last signal

        QSharedPointer<Reply> reply = arguments.at(0).value<QSharedPointer<Reply>>();
        // Check correct JSON
        QVERIFY2(reply->state().result() == Reply::NoError, reply->state().data().toLatin1());
        // Check correct fields
        BoolResult_t matches = reply->matches(*request.data());
        QVERIFY2(matches.result(), matches.data().toLatin1());
        QCOMPARE(reply->hdr()[Reply::IDS_RESULT].toInt(Const::INVALID), Const::SUCCESS);

        const qint32 cnt = reply->body()["RESULT"].toInt(Const::INVALID);
        QCOMPARE(cnt, 2);
    }

    {
        // Get notes back

        //-----------------------------------------------------------
        // Structure request
        //  HDR:
        //
        //  BODY:
        //      ts_from: int64 - low timestamp, default 0 (all records)
        //      ts_upto: int64 - top timestamp, default 0 (all records)
        //-----------------------------------------------------------
        // Structure reply
        //  HDR:
        //
        //  BODY:
        //      {Const::BODY_SET_NOTES : [array of Note-objects]}
        //-----------------------------------------------------------

        ord += 1;

        // Build request
        QScopedPointer<Request> request(new Request("fr@q.co", "psw01", Const::REQ_TYPE_GET_NOTES, ord, 1));
        QVERIFY2(request->state().result() == Request::NoError, request->state().data().toLatin1());

        QScopedPointer<HTTPClient> httpClient(new HTTPClient);
        QSignalSpy spy(httpClient.data(), SIGNAL(fireReply(QSharedPointer<Reply>)));
        httpClient->startRequest(QUrl(m_srvAddress), *request.data());
        QTest::qWait(HTTP_TIMEOUT_MSEC);
        QList<QVariant> arguments = spy.takeLast(); // take the last signal

        QSharedPointer<Reply> reply = arguments.at(0).value<QSharedPointer<Reply>>();
        // Check correct JSON
        QVERIFY2(reply->state().result() == Reply::NoError, reply->state().data().toLatin1());
        // Check correct fields
        BoolResult_t matches = reply->matches(*request.data());
        QVERIFY2(matches.result(), matches.data().toLatin1());
        QCOMPARE(reply->hdr()[Reply::IDS_RESULT].toInt(Const::INVALID), Const::SUCCESS);

        QJsonValue jvResult = reply->body()["RESULT"];
        QVERIFY(jvResult.isArray());
        QJsonArray result = jvResult.toArray();
        const qint32 cnt = result.count();
        QVERIFY(cnt==2);
        for(int i=0; i<cnt; ++i) {
            QSharedPointer<Note> n(Note::fromJsonObject(result.at(i).toObject()));
            QVERIFY(n != nullptr);
            Note *nn = noteVector.at(i).data();
            QVERIFY(nn->equalTo(n.data()) == true);
        }
    }


    {
        // Return with time borders

        {
            {
                // delete
                QScopedPointer<Request> request(new Request("fr@q.co", "psw01", Const::REQ_TYPE_DEL_NOTES, ++ord, 1));
                QVERIFY2(request->state().result() == Request::NoError, request->state().data().toLatin1());
                // Create http, send request
                QScopedPointer<HTTPClient> httpClient(new HTTPClient);
                QSignalSpy spy(httpClient.data(), SIGNAL(fireReply(QSharedPointer<Reply>)));
                httpClient->startRequest(QUrl(m_srvAddress), *request.data());
                QTest::qWait(HTTP_TIMEOUT_MSEC);
                // Get reply, check attributes
                QList<QVariant> arguments = spy.takeLast(); // take the last signal
                QSharedPointer<Reply> reply = arguments.at(0).value<QSharedPointer<Reply>>();
                // Check correct JSON
                QVERIFY2(reply->state().result() == Reply::NoError, reply->state().data().toLatin1());
                // Check correct fields
                BoolResult_t res = reply->matches(*request.data());
                QVERIFY2(res.result(), res.data().toLatin1());
                QCOMPARE(reply->hdr()[Reply::IDS_RESULT].toInt(Const::INVALID), Const::SUCCESS);

            }
            {
                // count
                QScopedPointer<Request> request(new Request("fr@q.co", "psw01", Const::REQ_TYPE_COUNT_NOTES, ord, 1));
                QVERIFY2(request->state().result() == Request::NoError, request->state().data().toLatin1());

                QScopedPointer<HTTPClient> httpClient(new HTTPClient);
                QSignalSpy spy(httpClient.data(), SIGNAL(fireReply(QSharedPointer<Reply>)));
                httpClient->startRequest(QUrl(m_srvAddress), *request.data());
                QTest::qWait(HTTP_TIMEOUT_MSEC);
                QList<QVariant> arguments = spy.takeLast(); // take the last signal

                QSharedPointer<Reply> reply = arguments.at(0).value<QSharedPointer<Reply>>();
                // Check correct JSON
                QVERIFY2(reply->state().result() == Reply::NoError, reply->state().data().toLatin1());
                // Check correct fields
                BoolResult_t matches = reply->matches(*request.data());
                QVERIFY2(matches.result(), matches.data().toLatin1());
                QCOMPARE(reply->hdr()[Reply::IDS_RESULT].toInt(Const::INVALID), Const::SUCCESS);

                const qint32 cnt = reply->body()["RESULT"].toInt(Const::INVALID);
                QCOMPARE(cnt, 0);
            }
        }

        {
            const qint32 max = 5;
            const qint32 times[max] = {10, 20, 30, 40, 50};

            {
                // send
                QScopedPointer<Request> request(new Request("fr@q.co", "psw01", Const::REQ_TYPE_ADD_NOTES, ord, 1));
                QVERIFY2(request->state().result() == Request::NoError, request->state().data().toLatin1());


                QJsonArray notes;
                for(int i=0; i<max; ++i) {
                    const QString uuid = Utils::createBase64Uuid();
                    QSharedPointer<Note> note(new Note(uuid));
                    noteVector.append(note);
                    note->setTsEdited(times[i]);
                    note->setAsDel(1);
                    note->setAuthorId(5);
                    note->setText(QString("? + % note_%1").arg(i));
                    QJsonObject jsonNote = note->toHttpJsonObject();
                    notes.append(jsonNote);
                    request->insert(Const::BODY_SET_NOTES, notes);
                }

                QScopedPointer<HTTPClient> httpClient(new HTTPClient);
                QSignalSpy spy(httpClient.data(), SIGNAL(fireReply(QSharedPointer<Reply>)));
                httpClient->startRequest(QUrl(m_srvAddress), *request.data());
                QTest::qWait(HTTP_TIMEOUT_MSEC);
                QList<QVariant> arguments = spy.takeLast(); // take the last signal

                QSharedPointer<Reply> reply = arguments.at(0).value<QSharedPointer<Reply>>();
                // Check correct JSON
                QVERIFY2(reply->state().result() == Reply::NoError, reply->state().data().toLatin1());
                // Check correct fields
                BoolResult_t matches = reply->matches(*request.data());
                QVERIFY2(matches.result(), matches.data().toLatin1());
                QCOMPARE(reply->hdr()[Reply::IDS_RESULT].toInt(Const::INVALID), Const::SUCCESS);
            }

            // retrieve
            {
                // 1. More than [2] = awaits 2
                QScopedPointer<Request> request(new Request("fr@q.co", "psw01", Const::REQ_TYPE_GET_NOTES, ++ord, 1));
                request->insert(Const::BODY_TS_EDIT_BOTTOM, times[2]);
                QVERIFY2(request->state().result() == Request::NoError, request->state().data().toLatin1());

                QScopedPointer<HTTPClient> httpClient(new HTTPClient);
                QSignalSpy spy(httpClient.data(), SIGNAL(fireReply(QSharedPointer<Reply>)));
                httpClient->startRequest(QUrl(m_srvAddress), *request.data());
                QTest::qWait(HTTP_TIMEOUT_MSEC);
                QList<QVariant> arguments = spy.takeLast(); // take the last signal

                QSharedPointer<Reply> reply = arguments.at(0).value<QSharedPointer<Reply>>();
                // Check correct JSON
                QVERIFY2(reply->state().result() == Reply::NoError, reply->state().data().toLatin1());
                // Check correct fields
                BoolResult_t matches = reply->matches(*request.data());
                QVERIFY2(matches.result(), matches.data().toLatin1());
                QCOMPARE(reply->hdr()[Reply::IDS_RESULT].toInt(Const::INVALID), Const::SUCCESS);

                QJsonValue jvResult = reply->body()["RESULT"];
                QVERIFY(jvResult.isArray());
                QJsonArray result = jvResult.toArray();
                const qint32 cnt = result.count();
                QCOMPARE(cnt, 2);
                for(int i=0; i<cnt; ++i) {
                    QSharedPointer<Note> n(Note::fromJsonObject(result.at(i).toObject()));
                    QVERIFY(n != nullptr);
                    qint32 ts_edit = n->tsEdited();
                    QVERIFY(ts_edit == times[3] || ts_edit == times[4]);
                }
            }

            {
                // 2. less then [3] = awaits 3
                QScopedPointer<Request> request(new Request("fr@q.co", "psw01", Const::REQ_TYPE_GET_NOTES, ++ord, 1));
                request->insert(Const::BODY_TS_EDIT_TOP, times[3]);
                QVERIFY2(request->state().result() == Request::NoError, request->state().data().toLatin1());

                QScopedPointer<HTTPClient> httpClient(new HTTPClient);
                QSignalSpy spy(httpClient.data(), SIGNAL(fireReply(QSharedPointer<Reply>)));
                httpClient->startRequest(QUrl(m_srvAddress), *request.data());
                QTest::qWait(HTTP_TIMEOUT_MSEC);
                QList<QVariant> arguments = spy.takeLast(); // take the last signal

                QSharedPointer<Reply> reply = arguments.at(0).value<QSharedPointer<Reply>>();
                // Check correct JSON
                QVERIFY2(reply->state().result() == Reply::NoError, reply->state().data().toLatin1());
                // Check correct fields
                BoolResult_t matches = reply->matches(*request.data());
                QVERIFY2(matches.result(), matches.data().toLatin1());
                QCOMPARE(reply->hdr()[Reply::IDS_RESULT].toInt(Const::INVALID), Const::SUCCESS);

                QJsonValue jvResult = reply->body()["RESULT"];
                QVERIFY(jvResult.isArray());
                QJsonArray result = jvResult.toArray();
                const qint32 cnt = result.count();
                QCOMPARE(cnt, 3);
                for(int i=0; i<cnt; ++i) {
                    QSharedPointer<Note> n(Note::fromJsonObject(result.at(i).toObject()));
                    QVERIFY(n != nullptr);
                    qint32 ts_edit = n->tsEdited();
                    QVERIFY(ts_edit == times[0] || ts_edit == times[1] || ts_edit == times[2]);
                }
            }

            {
                // 3. Between [1] and [3] = 3
                QScopedPointer<Request> request(new Request("fr@q.co", "psw01", Const::REQ_TYPE_GET_NOTES, ++ord, 1));
                request->insert(Const::BODY_TS_EDIT_BOTTOM, times[1]);
                request->insert(Const::BODY_TS_EDIT_TOP, times[3]);
                QVERIFY2(request->state().result() == Request::NoError, request->state().data().toLatin1());

                QScopedPointer<HTTPClient> httpClient(new HTTPClient);
                QSignalSpy spy(httpClient.data(), SIGNAL(fireReply(QSharedPointer<Reply>)));
                httpClient->startRequest(QUrl(m_srvAddress), *request.data());
                QTest::qWait(HTTP_TIMEOUT_MSEC);
                QList<QVariant> arguments = spy.takeLast(); // take the last signal

                QSharedPointer<Reply> reply = arguments.at(0).value<QSharedPointer<Reply>>();
                // Check correct JSON
                QVERIFY2(reply->state().result() == Reply::NoError, reply->state().data().toLatin1());
                // Check correct fields
                BoolResult_t matches = reply->matches(*request.data());
                QVERIFY2(matches.result(), matches.data().toLatin1());
                QCOMPARE(reply->hdr()[Reply::IDS_RESULT].toInt(Const::INVALID), Const::SUCCESS);

                QJsonValue jvResult = reply->body()["RESULT"];
                QVERIFY(jvResult.isArray());
                QJsonArray result = jvResult.toArray();
                const qint32 cnt = result.count();
                QCOMPARE(cnt, 3);
                for(int i=0; i<cnt; ++i) {
                    QSharedPointer<Note> n(Note::fromJsonObject(result.at(i).toObject()));
                    QVERIFY(n != nullptr);
                    qint32 ts_edit = n->tsEdited();
                    QVERIFY(ts_edit == times[3] || ts_edit == times[1] || ts_edit == times[2]);
                }
            }
        }
    }

    // TODO 16/07/05 Make server-sync in main app!
}


