#ifndef AUTHOR_H
#define AUTHOR_H

#include <QObject>

#include "basemodelitemlist.h"
#include "shared_result.h"
#include "shared_def.h"
using namespace vfx_shared;

namespace model {

class Author : public QObject
{
    Q_OBJECT
public:
    explicit Author(QObject *parent = 0);
    ~Author();

    qint32 id() const;
    void setId(const qint32 &id);

    QString title() const;
    void setTitle(const QString &title);

signals:

public slots:

private:
    qint32 m_id;
    QString m_title;
};

class AuthorList : public BaseModelItemList {
    Q_OBJECT
    typedef BaseModelItemList Inherited_t;
public:
    explicit AuthorList(QObject *parent = 0) : Inherited_t(parent){
        INC_THIS(true);
    }

    ~AuthorList(){
        foreach (auto item, m_items) {
            delete item;
        }
        DEC_THIS(true);
    }

    QList<Author*> &data() {
        return m_items;
    }

private:
    QList<Author*> m_items;

};

typedef Result<AuthorList*, QString> AuthorListResult_t;

} // namespace model;

Q_DECLARE_METATYPE(model::AuthorList*)

#endif // AUTHOR_H
