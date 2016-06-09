#ifndef NOTE_H
#define NOTE_H

#include <QObject>
#include <QSharedPointer>

#include "basemodelitemlist.h"

#include "shared_result.h"
#include "shared_def.h"
using namespace vfx_shared;

namespace model {

class Note : public QObject
{
    Q_OBJECT
public:
    explicit Note(QObject *parent = 0);
    ~Note();

    qint32 id() const;
    void setId(qint32 id);

    qint32 authorId() const;
    void setAuthorId(const qint32 &authorId);

    QString text() const;
    void setText(const QString &text);

    bool asDel() const;
    void setAsDel(bool asDel);

    qint64 tsCreated() const;
    void setTsCreated(const qint64 &tsCreated);

    qint64 tsEdited() const;
    void setTsEdited(const qint64 &tsEdited);

    QString toString() const;

signals:

public slots:

private:
    qint32 m_id;
    qint32 m_authorId;
    QString m_text;
    qint64 m_tsCreated;
    qint64 m_tsEdited;
    bool m_asDel;

    Note(const Note &n);
    Note &operator=(const Note &n);
};

struct Notes
{
    explicit Notes(){
//        qRegisterMetaType<QSharedPointer<model::Notes>>("QSharedPointer<model::Notes>");
    }
    ~Notes(){
        foreach (auto n, items) {
            delete n;
        }
        items.clear();
    }
    QVector<Note*> items;
private:
    Notes &operator=(Notes &);
    Notes(Notes &);
};

} // namespace model;

Q_DECLARE_METATYPE(QSharedPointer<model::Notes>)


#endif // NOTE_H
