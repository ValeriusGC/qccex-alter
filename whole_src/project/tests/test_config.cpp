#include "test_config.h"
#include "settings.h"

using namespace nq;

void Test_Config::initTestCase()
{
    m_key = "testWrite";
    m_value = "testValue";
    QVERIFY("testValue" == m_value);
}

void Test_Config::testWrite()
{
    Settings *s = &Settings::instance();
    s->setValue(m_key, m_value);
    QString value = s->value(m_key, "").toString();
    QVERIFY(m_value == value);
}

void Test_Config::cleanupTestCase()
{
    Settings *s = &Settings::instance();
    s->remove(m_key);
    QVERIFY(s->allKeys().contains(m_key) == false);
}

