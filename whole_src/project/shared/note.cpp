#include <QUrl>
#include <QJsonDocument>

#include "note.h"
#include "author.h"
#include "storage_globals.h"

using namespace model;

Note::Note(const QString &uuid, QObject *parent)
    : QObject(parent),
      m_authorId(0),
      m_asDel(0),
      m_valid(true, QList<QString>())
{
    INC_THIS(false);
    Q_ASSERT_X(!uuid.isEmpty(), Q_FUNC_INFO, "UUID should be correct");

    m_uuid = uuid;

    const qint64 ts = QDateTime::currentMSecsSinceEpoch();
    m_tsCreated = ts;
    m_tsEdited = ts;
    //    LOG_TP(m_text << m_uuid);
}

Note::~Note()
{
//    LOG_TP(m_uuid << m_text);
    DEC_THIS(false);
}

Note::Note(const Note &n) : QObject(n.parent()), m_valid(true, QList<QString>())
{
    INC_THIS(false);
    m_uuid = n.m_uuid;
    m_authorId = n.m_authorId;
    m_text = n.m_text;
    m_tsCreated = 0;
    m_tsEdited = 0;
    m_asDel = 0;
}

Note &Note::operator=(const Note &n)
{
    m_uuid = n.m_uuid;
    m_authorId = n.m_authorId;
    m_text = n.m_text;
    return *this;
}

QString Note::uuid() const
{
    return m_uuid;
}

qint64 Note::authorId() const
{
    return m_authorId;
}

void Note::setAuthorId(const qint64 &authorId)
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
    return QString("%1: %2 (%3/%4)").arg(m_uuid).arg(trunc)
            .arg(QDateTime::fromMSecsSinceEpoch(m_tsCreated).toString())
            .arg(QDateTime::fromMSecsSinceEpoch(m_tsEdited).toString());
}

Note *Note::copy() const
{
    Note *res = new Note(m_uuid);
    res->m_rowid = m_rowid;
    res->m_authorId = m_authorId;
    res->m_text = m_text;
    res->m_tsCreated = m_tsCreated;
    res->m_tsEdited = m_tsEdited;
    res->m_asDel = m_asDel;
    res->m_valid = m_valid;
    return res;
}

bool Note::equalTo(Note *note) const
{
    if(note->uuid() != m_uuid){
        return false;
    }
    if(note->asDel() != m_asDel){
        return false;
    }
    if(note->text() != m_text){
        return false;
    }
    if(note->authorId() != m_authorId){
        return false;
    }
    if(note->tsCreated() != m_tsCreated){
        return false;
    }
    if(note->tsEdited() != m_tsEdited){
        return false;
    }

    return true;
}

QJsonObject Note::toHttpJsonObject()
{
    QJsonObject body;
    using namespace storage;
    body[Const::FLD_UUID] = QString(QUrl::toPercentEncoding(m_uuid));
    body[Const::FLD_DEL] = m_asDel;
    body[Const::FLD_AUTHOR_REF] = m_authorId;
    body[Const::FLD_TS_CREATE] = m_tsCreated;
    body[Const::FLD_TS_EDIT] = m_tsEdited;
    body[Const::FLD_THE_TEXT] = QString(QUrl::toPercentEncoding(m_text));
    return body;
}

QJsonObject Note::toJsonObject()
{
    QJsonObject body;
    using namespace storage;
    body[Const::FLD_UUID] = m_uuid;
    body[Const::FLD_DEL] = m_asDel;
    body[Const::FLD_AUTHOR_REF] = m_authorId;
    body[Const::FLD_TS_CREATE] = m_tsCreated;
    body[Const::FLD_TS_EDIT] = m_tsEdited;
    body[Const::FLD_THE_TEXT] = m_text;
    return body;
}

Note *Note::fromJsonObject(const QJsonObject &obj)
{
    if(obj.isEmpty()) {
        return nullptr;
    }

    using namespace storage;
    const QString uuid = obj[Const::FLD_UUID].toString();
    Note *note = new Note(uuid);
    {
        QJsonValue jv = obj[Const::FLD_DEL];
        if(jv.isString()){
            note->setAsDel(obj[Const::FLD_DEL].toString().toInt());
        }else if(jv.isBool()) {
            note->setAsDel(obj[Const::FLD_DEL].toBool());
        }
    }
    {
        const QString name = Const::FLD_AUTHOR_REF;
        const QString s = obj[Const::FLD_AUTHOR_REF].toString();

        QJsonValue jv = obj[Const::FLD_AUTHOR_REF];
        if(jv.isString()){
            note->setAuthorId(obj[Const::FLD_AUTHOR_REF].toString().toInt());
        }else if(jv.isDouble()) {
            note->setAuthorId(obj[Const::FLD_AUTHOR_REF].toInt());
        }
    }

    note->setText(obj[Const::FLD_THE_TEXT].toString());

    {
        QJsonValue jv = obj[Const::FLD_TS_CREATE];
        if(jv.isString()){
            note->setTsCreated(obj[Const::FLD_TS_CREATE].toString().toDouble());
        }else if(jv.isDouble()) {
            note->setTsCreated(obj[Const::FLD_TS_CREATE].toDouble());
        }
    }
    {
        QJsonValue jv = obj[Const::FLD_TS_EDIT];
        if(jv.isString()){
            note->setTsEdited(obj[Const::FLD_TS_EDIT].toString().toDouble());
        }else if(jv.isDouble()) {
            note->setTsEdited(obj[Const::FLD_TS_EDIT].toDouble());
        }
    }

    return note;
}


