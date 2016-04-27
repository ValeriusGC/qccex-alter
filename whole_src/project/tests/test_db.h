#ifndef TEST_DB_H
#define TEST_DB_H

#include <QObject>

#include "alltests.h"

class Test_DB : public QObject
{
    Q_OBJECT
private slots:
    // Auto-initializator
    void initTestCase();

    /**
     * @brief testVersion
     *
     * Test that DB has expected version.
     *
     */
    void testVersion();

    // Auto-cleaner
    void cleanupTestCase();
};

DECLARE_TEST(Test_DB)

#endif // TEST_DB_H
