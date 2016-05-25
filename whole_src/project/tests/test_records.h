#ifndef TESTRECORDS_H
#define TESTRECORDS_H

#include <QObject>

#include "alltests.h"
#include "sqlitestorage.h"
#include "note.h"

using namespace storage;

/**
 *  Tests adding, removing records.
 * @brief The TestRecords class
 */
class TestRecords : public QObject
{
    Q_OBJECT
public:
    explicit TestRecords(QObject *parent = 0);
    ~TestRecords();

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

    void testAddNote();

private:
    /**
     *  Time to init
     * @brief m_initTime
     */
    qint32 m_waitTime;

    QString m_dbName;

    SqliteStorage *m_storage;

    NoteList *m_notes;
};

DECLARE_TEST(TestRecords)

#endif // TESTRECORDS_H
