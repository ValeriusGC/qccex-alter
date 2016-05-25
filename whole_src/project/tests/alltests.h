#ifndef ALLTESTS_H
#define ALLTESTS_H

#include <QTest>
#include <QMap>
#include <QString>
#include <QSharedPointer>

namespace AutoTest{

typedef QMap<QString, QObject*> TestList;

inline TestList& testList() {
    static TestList list;
    return list;
}

inline bool findObject(const QString &name) {
    TestList& list = testList();
    if (list.contains(name)) {
        return true;
    }
//    foreach (QObject* test, list) {
//        if (test->objectName() == object->objectName()) {
//            return true;
//        }
//    }
    return false;
}

inline void addTest(const QString &name, QObject* object) {
    TestList& list = testList();
    if (!findObject(name)) {
        list.insert(name, object);
    }
}

inline int run(int argc, char *argv[]) {
    int ret = 0;
    foreach (QObject* test, testList()) {
        ret += QTest::qExec(test, argc, argv);
    }
    return ret;
}

} // namespace AutoTest

template <class T>
class Test
{
public:
    QSharedPointer<T> child;

    Test(const QString& name) {
        if(!AutoTest::findObject(name)) {
            child.reset(new T);
            child->setObjectName(name);
            AutoTest::addTest(name, child.data());
        }
    }
};

#define DECLARE_TEST(className) static Test<className> t(#className);

#define TEST_MAIN \
    int main(int argc, char *argv[]) \
    { \
        QApplication a(argc, argv); \
        return AutoTest::run(argc, argv); \
    }

#endif // ALLTESTS_H
