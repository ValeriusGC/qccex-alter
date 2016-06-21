#ifndef UTILS_H
#define UTILS_H

#include <QtCore>

class Utils
{
public:

    static QString createBase64Uuid();

private:
    Utils();
    ~Utils();
};

#endif // UTILS_H
