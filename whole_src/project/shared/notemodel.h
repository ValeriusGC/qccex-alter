#ifndef NOTEMODEL_H
#define NOTEMODEL_H

#include <QAbstractListModel>

#include "note.h"
#include "base_storage.h"

using namespace storage;

namespace model {

class NoteModel : public QAbstractListModel
{
    Q_OBJECT
    typedef QAbstractListModel Inherited_t;
public:
    explicit NoteModel(QObject *parent = 0);
    ~NoteModel();

    // QAbstractItemModel interface

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

    // ~QAbstractItemModel interface

    QList<Note*> &items() {
        return m_items;
    }

signals:

public slots:
    void add(Note *note);

private slots:
    void onTaskProgress(const nq::ProgressInfo &pi, const QVariant &sp);

private:
    QList<Note*> m_items;
    BaseStorage *m_storage;

};

} // namespace model

Q_DECLARE_METATYPE(model::NoteModel*)


#endif // NOTEMODEL_H
