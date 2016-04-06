#include <QApplication>

#include "widget.h"

#include "note_defines.h"


#ifdef DO_TESTS
    #include "alltests.h"
    // This is all you need to run all the tests
    TEST_MAIN
#else

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LOG_TP(QObject::tr("Hello"));

    Widget w;
    w.show();

    return a.exec();
}

#endif
