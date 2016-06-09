#include "note.h"
#include "author.h"

using namespace model;

Note::Note(QObject *parent)
    : QObject(parent),
      m_id(0),
      m_authorId(0),
      m_tsCreated(0),
      m_tsEdited(0),
      m_asDel(0)
{
    INC_THIS(false);
}

Note::~Note()
{
    DEC_THIS(false);
}

Note::Note(const Note &n) : QObject(n.parent())
{
    INC_THIS(false);
    m_id = n.m_id;
    m_authorId = n.m_authorId;
    m_text = n.m_text;
    m_tsCreated = 0;
    m_tsEdited = 0;
    m_asDel = 0;
}

Note &Note::operator=(const Note &n)
{
    m_id = n.m_id;
    m_authorId = n.m_authorId;
    m_text = n.m_text;
    return *this;
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

bool Note::asDel() const
{
    return m_asDel;
}

void Note::setAsDel(bool asDel)
{
    m_asDel = asDel;
}

qint64 Note::tsCreated() const
{
    return m_tsCreated;
}

void Note::setTsCreated(const qint64 &tsCreated)
{
    m_tsCreated = tsCreated;
}

qint64 Note::tsEdited() const
{
    return m_tsEdited;
}

void Note::setTsEdited(const qint64 &tsEdited)
{
    m_tsEdited = tsEdited;
}

QString Note::toString() const
{
    QString trunc = QString(m_text);
    trunc.truncate(5);
    return QString("%1: %2 (%3/%4)").arg(m_id).arg(trunc)
            .arg(QDateTime::fromMSecsSinceEpoch(m_tsCreated).toString())
            .arg(QDateTime::fromMSecsSinceEpoch(m_tsEdited).toString());
}




