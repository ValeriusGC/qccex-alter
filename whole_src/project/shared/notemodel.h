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
    enum Roles {
        NMR_Id = Qt::UserRole + 1,
        NMR_TsCreated,
        NMR_TsEdited,
        NMR_Del,
        NMR_Debug
    };

    explicit NoteModel(QObject *parent = 0);
    ~NoteModel();

    // QAbstractItemModel interface

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

    // ~QAbstractItemModel interface

    QSharedPointer<Notes> &items() {
        return m_items;
    }
    //    QList<Note*> &items() {
    //        return m_items;
    //    }

signals:

public slots:
    void add(Note *note);
    void markAsDeleted(qint32 id);
    void remove(qint32 id);

private slots:
    void onTaskProgress(const nq::ProgressInfo &pi, const QVariant &sp);
    void onTaskNoteProgress(const nq::ProgressInfo &pi, const QSharedPointer<model::Notes> &notes);

private:
    //    QList<Note*> m_items;
    QHash<int, QByteArray> m_roles;
    QSharedPointer<Notes> m_items;
    BaseStorage *m_storage;

};


class MySortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    MySortFilterProxyModel(QObject *parent = 0);
protected:
    // QSortFilterProxyModel interface
    virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
private:

};

} // namespace model

//Q_DECLARE_METATYPE(model::NoteModel*)


#endif // NOTEMODEL_H
