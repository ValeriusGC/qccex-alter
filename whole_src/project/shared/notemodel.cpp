#include <QBrush>
#include <QColor>
#include <QMetaType>

#include "notemodel.h"
#include "shared_def.h"
#include "sqlitestorage.h"

using namespace model;
using namespace vfx_shared;
using nq::ProgressInfo;

NoteModel::NoteModel(QObject *parent)
    : Inherited_t(parent)
{
    INC_THIS(false);
    // qRegisterMetaType for queue signals/slots between threads.
    qRegisterMetaType< QSharedPointer<model::Notes> >("QSharedPointer<model::Notes>");

    m_roles[NMR_Id] = "id";
    m_roles[NMR_Del] = "del";

    m_items = QSharedPointer<Notes>::create();
    m_storage = &SqliteStorage::instance();
    if(m_storage->isInit()) {
        connect(m_storage, SIGNAL(fireTaskProgress(nq::ProgressInfo,QVariant)),
                SLOT(onTaskProgress(nq::ProgressInfo,QVariant)));
        connect(m_storage, SIGNAL(fireTaskNoteProgress(nq::ProgressInfo,QSharedPointer<model::Notes>)),
                SLOT(onTaskNoteProgress(nq::ProgressInfo,QSharedPointer<model::Notes>)));
        m_storage->fetchNotes(TASK_ID_FETCH_NOTES, UuidVector_t());
    }

}

NoteModel::~NoteModel()
{
    //    foreach (auto item, m_items) {
    //        delete item;
    //    }
    DEC_THIS(false);
}

int NoteModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_items->items.size();
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

    if (indexRow < 0 || indexRow > m_items->items.size()){
        return QVariant();
    }

    Note *item = m_items->items[indexRow];
    //    const QString dt = QDateTime::fromMSecsSinceEpoch(item.msecs()).toString(QString("MM-dd hh:mm:ss.zzz"));
    const bool odd = indexRow % 2;

    switch (role) {
    case NMR_Id:
        return item->uuid();
    case NMR_TsCreated:
        return item->tsCreated();
    case NMR_TsEdited:
        return item->tsEdited();
    case Qt::DisplayRole:
        return item->text();
    case NMR_Debug:
        return item->toString();
    case Qt::TextColorRole:
        return QBrush( item->asDel() ? QColor(255,0,0) : QColor(0, 0, 0));
    case Qt::BackgroundColorRole:
        return QBrush(odd ? QColor(QColor(240,240,240)) : QColor(Qt::white));
    default:
        break;
    }
    return QVariant();

}

void NoteModel::add(Note *note)
{
    QSharedPointer<Notes> sp(new Notes);
    sp->items.append(note);
    m_storage->addNotes(10, sp);
}

void NoteModel::markAsDeleted(const UuidType_t &id)
{
    UuidVector_t ids;
    ids.append(id);
    m_storage->markNotesAsDeleted(20, ids, true);
}

void NoteModel::remove(const UuidType_t &id)
{
    Q_UNUSED(id);
}

void NoteModel::onTaskProgress(const nq::ProgressInfo &pi, const QVariant &sp)
{
    if(pi.status == ProgressInfo::TPS_Success) {
        if(pi.id == TASK_ID_FETCH_NOTES) {
            QSharedPointer<Notes> spnl = sp.value< QSharedPointer<Notes> >();
            beginResetModel();



//            const qint32 selRow =

            foreach (auto item, m_items->items) {
                delete item;
            }
            m_items->items.clear();
            Notes *notes = spnl.data();
            while(notes->items.count()){
                m_items->items.append(notes->items.takeFirst());
            }
            endResetModel();
        }else{
            m_storage->fetchNotes(TASK_ID_FETCH_NOTES, UuidVector_t());
        }
    }
}

void NoteModel::onTaskNoteProgress(const nq::ProgressInfo &pi, const QSharedPointer<Notes> &notes)
{
    if(pi.id==10 && pi.status == ProgressInfo::TPS_Success) {
        beginInsertRows(QModelIndex(), m_items->items.size()-1, 1);
        while(notes->items.count()){
            m_items->items.append(notes->items.takeFirst());
        }
        endInsertRows();
    }
}






MySortFilterProxyModel::MySortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    LOG;
}

bool MySortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    QVariant leftData = left.data(NoteModel::NMR_TsCreated);
    QVariant rightData = right.data(NoteModel::NMR_TsCreated);
    if (leftData.type() == QVariant::LongLong) {
        return leftData.toLongLong() < rightData.toLongLong();
    } else {
        QString leftString = leftData.toString();
        QString rightString = rightData.toString();
        return QString::localeAwareCompare(leftString, rightString) < 0;
    }
}
