#include <QTest>
#include <QApplication>

#include "alltests.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    return AutoTest::run(argc, argv);
}
