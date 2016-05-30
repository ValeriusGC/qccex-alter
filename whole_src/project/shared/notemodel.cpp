#include <QBrush>
#include <QColor>

#include "notemodel.h"
#include "shared_def.h"
#include "storagefactory.h"

using namespace model;
using namespace vfx_shared;
using nq::ProgressInfo;

NoteModel::NoteModel(QObject *parent) : Inherited_t(parent)
{
    INC_THIS(true);
    // TODO remove new SqliteStorage(); from this because it breaks tests
    m_storage = StorageFactory::instance().make("sqlite");
    if(m_storage && m_storage->isInit()) {
        connect(m_storage, SIGNAL(fireTaskProgress(nq::ProgressInfo,QVariant)), SLOT(onTaskProgress(nq::ProgressInfo,QVariant)));
    }
}

NoteModel::~NoteModel()
{
    foreach (auto item, m_items) {
        delete item;
    }
    DEC_THIS(true);
}

int NoteModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_items.size();
}

QVariant NoteModel::data(const QModelIndex &index, int role) const
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

    Note *item = m_items[indexRow];
//    const QString dt = QDateTime::fromMSecsSinceEpoch(item.msecs()).toString(QString("MM-dd hh:mm:ss.zzz"));
    const bool odd = indexRow % 2;

    switch (role) {
    case Qt::DisplayRole:
        return item->text();
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

void NoteModel::add(Note *note)
{
    NoteModel *nl = new NoteModel;
    nl->items().append(note);
    m_storage->addNotes(10, QSharedPointer<NoteModel>(nl));
}

void NoteModel::onTaskProgress(const nq::ProgressInfo &pi, const QVariant &sp)
{
    if(pi.id==10 && pi.status == ProgressInfo::TPS_Success) {
        QSharedPointer<QObject> spo = sp.value< QSharedPointer<QObject> >();
        QSharedPointer<NoteModel> spnl = qSharedPointerDynamicCast<NoteModel>(spo);
        beginInsertRows(QModelIndex(), m_items.size()-1, 1);
        foreach (auto item, spnl.data()->items()) {
            // TODO make adding correctly! (without this copying!!!)
            Note *note = new Note;
            note->setText(item->text());
            m_items.append(note);
        }
        endInsertRows();
    }
}

