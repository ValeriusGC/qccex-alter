#include <QApplication>
#include <QMetaType>

#include "widget.h"
#include "progressinfo.h"

#include "shared/shared_def.h"
using namespace vfx_shared;

#ifdef DO_TESTS
    #include "alltests.h"
    // This is all you need to run all the tests
    TEST_MAIN
#else

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<nq::ProgressInfo>("nq::ProgressInfo");
    int retCode;
    {
        Widget w;
        w.show();
        retCode = a.exec();
    }
    // Settings::instance (and maybe other static singletones must be in memory yet in this point due their behaviour)
    CHECK_PTR;
    return retCode;
}

#endif
