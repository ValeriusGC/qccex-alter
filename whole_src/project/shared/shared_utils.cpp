#include <QUuid>

#include "shared_utils.h"

QString Utils::createBase64Uuid()
{
    return QUuid::createUuid().toRfc4122().toBase64();
}

Utils::Utils()
{

}

Utils::~Utils()
{

}

