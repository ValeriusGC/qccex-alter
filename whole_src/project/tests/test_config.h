#ifndef TEST_CONFIG_H
#define TEST_CONFIG_H

#include <QObject>

#include "alltests.h"

/**
 * @brief The Test_Config class
 *
 * Tests for configuration file
 *
 */
class Test_Config : public QObject
{
    Q_OBJECT
private slots:

    // Auto-initializator
    void initTestCase();

    void testWrite();

    // Auto-cleaner
    void cleanupTestCase();

private:
    QString m_key;
    QString m_value;
};

DECLARE_TEST(Test_Config)

#endif // TEST_CONFIG_H
