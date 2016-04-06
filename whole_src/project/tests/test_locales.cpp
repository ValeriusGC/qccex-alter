#include "test_locales.h"
#include "settings.h"

void Test_Locales::testFiles()
{
    const int await_cnt = 3;
    // 1. Verify there are 4 files.
    const QHash<QString, QString> files = nq::Settings::instance().enumLanguages();
    const int cnt = files.count();
    QCOMPARE(cnt, await_cnt);

    // 2. Verify their locale codes;
    const QVector<QString> names = {"ru", "uk"};
    QCOMPARE(names.count(), await_cnt);

    QVERIFY(files.contains(names.at(0)));
    QVERIFY(files.contains(names.at(1)));
}

