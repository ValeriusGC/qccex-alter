#ifndef TESTRECORDS_H
#define TESTRECORDS_H

#include <QObject>

#include "alltests.h"
#include "sqlitestorage.h"
#include "notemodel.h"
#include "progressinfo.h"
using namespace nq;

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

    void testAddNote3();


private:
    /**
     *  Time to init
     * @brief m_initTime
     */
    qint32 m_waitTime;

    QString m_dbName;

    SqliteStorage *m_storage;

    model::NoteModel *m_notes;
};

DECLARE_TEST(TestRecords)

#endif // TESTRECORDS_H
