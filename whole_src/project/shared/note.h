#ifndef NOTE_H
#define NOTE_H

#include <QObject>
#include <QSharedPointer>
QT_BEGIN_NAMESPACE
class QUuid;
QT_END_NAMESPACE


#include "basemodelitemlist.h"

#include "shared_result.h"
#include "shared_def.h"
using namespace vfx_shared;

namespace model {
/**
 * @brief The Note class
 *
 */
class Note : public QObject
{
    Q_OBJECT
public:
    explicit Note(const QString &uuid, QObject *parent = 0);
    ~Note();

    /**
     * @brief uuid
     * @return QUUID formatted as RFC4122 and then as BASE64 (24 bytes long instead of 38 as original QUUID)
     */
    QString uuid() const;

    qint64 authorId() const;
    void setAuthorId(const qint64 &authorId);

    QString text() const;
    void setText(const QString &text);

    bool asDel() const;
    void setAsDel(bool asDel);

    qint64 tsCreated() const;
    void setTsCreated(const qint64 &tsCreated);

    qint64 tsEdited() const;
    void setTsEdited(const qint64 &tsEdited);

    QString toString() const;

    /**
     * @brief copy
     * @return Copy of this object
     */
    Note *copy() const;

signals:

public slots:

private:
    // TODO. 16/06/13  I did BASE64. Now one need to create Undo/Redo for records.

    // TODO How to apply rowid in my case? One need to think.
    /**
     * Unique system field for all tables in SQLITE.
     * See @link https://habrahabr.ru/post/160861/
     * Useful for quick searching in tables.
     *
     * @brief m_rowid
     */
    qint64 m_rowid;

    /**
     * @brief m_uuid
     * Stores UUID in RFC4122 form and formatted as BASE64
     */
    QString m_uuid;
    qint64 m_authorId;
    QString m_text;
    qint64 m_tsCreated;
    qint64 m_tsEdited;
    bool m_asDel;
    BoolListResult_t m_valid;

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
