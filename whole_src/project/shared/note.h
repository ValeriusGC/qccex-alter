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

signals:

public slots:

private:
    qint32 m_id;
    qint32 m_authorId;
    QString m_text;
};

//class NoteList : public BaseModelItemList {
//    Q_OBJECT
//    typedef BaseModelItemList Inherited_t;
//public:

//    explicit NoteList(QObject *parent = 0) : Inherited_t(parent){
//        INC_THIS(true);
//    }

//    ~NoteList(){
//        foreach (auto item, m_items) {
//            delete item;
//        }
//        DEC_THIS(true);
//    }

//    QList<Note*> &data() {
//        return m_items;
//    }

//private:
//    QList<Note*> m_items;

//};

//typedef Result<NoteList*, QString> NoteListResult_t;

//class TaskGetNoteList {
//public:
//private:
//    qint64 m_id;
//    QSharedPointer<NoteList> m_p;
//};

} // namespace model;

//Q_DECLARE_METATYPE(model::NoteList)
//Q_DECLARE_METATYPE(model::NoteList*)
//Q_DECLARE_METATYPE(model::Note*)


#endif // NOTE_H
