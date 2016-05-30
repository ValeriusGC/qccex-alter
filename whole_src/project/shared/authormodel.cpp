#include <QBrush>
#include <QColor>

#include "authormodel.h"

#include "shared_def.h"
using namespace vfx_shared;

using namespace model;

AuthorModel::AuthorModel(QObject *parent) : QObject(parent)
{
    INC_THIS(true);
}

AuthorModel::~AuthorModel()
{
    foreach (auto item, m_items) {
        delete item;
    }
    DEC_THIS(true);
}

int AuthorModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_items.size();
}

QVariant AuthorModel::data(const QModelIndex &index, int role) const
{
    // What called in roles:
    // DecorationRole = 1,
    // DisplayRole = 0,
    // BackgroundColorRole = 8,
    // FontRole = 6,
    // TextAlignmentRole = 7,
    // TextColorRole = 9 or ForegroundRole = 9,
    // CheckStateRole = 10,

    const int indexRow = index.row();

    if (indexRow < 0 || indexRow > m_items.size()){
        return QVariant();
    }

    auto *item = m_items[indexRow];
//    const QString dt = QDateTime::fromMSecsSinceEpoch(item.msecs()).toString(QString("MM-dd hh:mm:ss.zzz"));
    const bool odd = indexRow % 2;

    switch (role) {
    case Qt::DisplayRole:
        return item->title();
//        return QString("%1\t%2\t%3").arg(dt).arg(LogItem::levelToString(item.level())).arg(item.message());
//    case Qt::TextColorRole:
//        return QBrush( item.level() >= LogItem::LL_Warning ? QColor(255,0,0) : QColor(0, 0, 0));
    case Qt::BackgroundColorRole:
        return QBrush(odd ? QColor(QColor(240,240,240)) : QColor(Qt::white));
    default:
        break;
    }
    return QVariant();
}

