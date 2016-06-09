#include <QTest>
#include <QApplication>

#include "alltests.h"
#include "shared_def.h"
using namespace vfx_shared;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    int retCode;
    {
        retCode = AutoTest::run(argc, argv);
    }
    // Settings::instance (and maybe other static singletones must be in memory yet in this point due their behaviour)
    CHECK_PTR;
    return retCode;
}
