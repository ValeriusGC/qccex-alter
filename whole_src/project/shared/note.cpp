#include "note.h"
#include "author.h"

using namespace model;

Note::Note(QObject *parent) : QObject(parent)
{
//    Q_ASSERT_X(authorId > 0, Q_FUNC_INFO, "Author id not valid");
//    m_id = id;
//    m_authorId = authorId;
    INC_THIS(this);
}

Note::~Note()
{
    DEC_THIS(true);
}

qint32 Note::id() const
{
    return m_id;
}

void Note::setId(qint32 id)
{
    m_id = id;
}

qint32 Note::authorId() const
{
    return m_authorId;
}

void Note::setAuthorId(const qint32 &authorId)
{
    m_authorId = authorId;
}

QString Note::text() const
{
    return m_text;
}

void Note::setText(const QString &text)
{
    m_text = text;
}

