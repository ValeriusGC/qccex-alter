#ifndef TEST_LOCALES_H
#define TEST_LOCALES_H

#include <QObject>

#include "alltests.h"

/**
 * @brief The Test_Locales class
 *
 * Tests existing locale codes in resources.
 *
 * There must be 4 codes:
 * ru for russian (note_qt_ru.qm),
 * uk for ukrainian (note_qt_uk.qm),
 * pl for poland (note_qt_pl.qm),
 * cs for chech (note_qt_cs.qm)
 */
class Test_Locales : public QObject
{
    Q_OBJECT
private slots:
//    void initTestCase();
    void testFiles();
//    void cleanupTestCase();
};

DECLARE_TEST(Test_Locales)


#endif // TEST_LOCALES_H
