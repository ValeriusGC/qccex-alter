#ifndef TESTHTTPCONNECT_H
#define TESTHTTPCONNECT_H

#include <QObject>

#include "alltests.h"

class TestHttpConnect : public QObject
{
    Q_OBJECT
public:
    explicit TestHttpConnect(QObject *parent = 0);
    ~TestHttpConnect();

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
     * @brief testRequest
     * Simple test for Request.
     *  1. Check when CID/PSW is empty
     *  2. Check collaboration with Reply
     */
    void testRequest();


    /**
     * @brief testJson
     *  Build Json
     */
    void testJson();

    /**
     * @brief testJsonParser
     *  Tests JsonParser object.
     */
    void testJsonParser();

    /**
     * @brief testHttpHandshake
     *  Sends some variants of 'hello' requests.
     *  Gets results.
     */
    void testHttpHandshake();

    /**
     * @brief testQueryForNotes
     *  Deleting/Adding/Retrieving notes
     *  Prepare Request for list of notes.
     */
    void testQueryForNotes();

private:
    /**
     * @brief m_srvAddress
     *  Server address.
     */
    QString m_srvAddress;
    /**
     * @brief m_account
     *  Client account
     */
    QString m_account;
    /**
     * @brief m_psw
     *  Client password
     */
    QString m_psw;
};

DECLARE_TEST(TestHttpConnect)

#endif // TESTHTTPCONNECT_H
