#ifndef NOTEITEMDELEGATE_H
#define NOTEITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

namespace nq {

class NoteItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    typedef QStyledItemDelegate Inherited_t;
public:
    explicit NoteItemDelegate(const QString &templ, QObject *parent = 0);
    ~NoteItemDelegate();

    // QAbstractItemDelegate interface
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:

public slots:

private:
    QString m_template;
    QString m_output;

};

} // namespace nq

#endif // NOTEITEMDELEGATE_H
