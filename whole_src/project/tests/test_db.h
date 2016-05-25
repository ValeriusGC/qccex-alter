#ifndef TEST_DB_H
#define TEST_DB_H

#include <QObject>
#include <QSignalSpy>

#include "alltests.h"
#include "shared_result.h"

/**
 *  Tests:
 *  1. When create from scratch (check versions -1, 0, 1, 2, 3)
 *      1.1.    With invalid number (<1) fires only OnError
 *      1.2.    With correct number (>=1)
 *          1.2.1. Fires only OnCreate
 *          1.2.2. Has correct properties
 *              - version
 *              - init-flag
 *              - versions in list of tables
 *  2. When upgrade (check versions -1, 0, 1, 2, 3)
 *      2.1.    With invalid number (<1) fires only OnError
 *      2.2.    With downgrade progression fires only OnError
 *      2.3.    With correct number and progression
 *          2.3.1. Fires only OnUpgrade
 *          2.3.2. Has correct properties
 *              - version
 *              - init-flag
 *              - versions in list of tables
 * @brief The Test_DB class
 */
class Test_DB : public QObject
{
    Q_OBJECT

public:

//
//    explicit Test_DB(QObject *parent = 0);
//    Test_DB(Test_DB &r);
//    virtual ~Test_DB();
//    Test_DB &operator=(Test_DB &r);


private slots:
//    // Auto-initializator for entire object
//    void initTestCase();
//    // Auto-cleaner for entire object
//    void cleanupTestCase();
    // Auto-initializator for every test-function
    void init();
    // Auto-cleaner for every test-function
    void cleanup();

    /**
     *  Check with version 0.
     *  This is invalid version.
     * @brief testCreateWithInvalidVersionZero
     */
    void testCreateWithInvalidVersionZero();
    /**
     *  Check with version -1.
     *  This is invalid version.
     * @brief testCreateWithInvalidVersionNegative
     */
    void testCreateWithInvalidVersionNegative();

    /**
     *  Check with version 1.
     *  This is correct version.
     * @brief testCreateWithCorrectV1
     */
    void testCreateWithCorrectV1();

    /**
     *  Check with version 2.
     *  This is correct version.
     * @brief testCreateWithCorrectV2
     */
    void testCreateWithCorrectV2();

private:
    enum SignalSpy {
        SS_OnError,
        SS_OnCreate,
        SS_OnUpgrade
    };

    QString m_dbName;

    QVector<QSignalSpy*> m_spies;

};

//DECLARE_TEST(Test_DB)

#endif // TEST_DB_H
