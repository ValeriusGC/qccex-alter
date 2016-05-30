#ifndef AUTHORMODEL_H
#define AUTHORMODEL_H

#include <QAbstractListModel>

#include "author.h"

namespace model {

class AuthorModel : public QObject
{
    Q_OBJECT
    typedef QAbstractListModel Inherited_t;
public:
    explicit AuthorModel(QObject *parent = 0);
    ~AuthorModel();

    // QAbstractItemModel interface

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

    // ~QAbstractItemModel interface

    QList<Author*> &data() {
        return m_items;
    }
signals:

public slots:

private:
    QList<Author*> m_items;
};

} // namespace model

#endif // AUTHORMODEL_H
