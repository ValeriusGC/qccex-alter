#include "author.h"

using namespace model;

Author::Author(QObject *parent) : QObject(parent)
{

}

Author::~Author()
{

}
qint32 Author::id() const
{
    return m_id;
}

void Author::setId(const qint32 &id)
{
    m_id = id;
}
QString Author::title() const
{
    return m_title;
}

void Author::setTitle(const QString &title)
{
    m_title = title;
}



