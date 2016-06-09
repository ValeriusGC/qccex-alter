#include <QBrush>
#include <QColor>
#include <QMetaType>

#include "notemodel.h"
#include "shared_def.h"
#include "storagefactory.h"

using namespace model;
using namespace vfx_shared;
using nq::ProgressInfo;

NoteModel::NoteModel(QObject *parent)
    : Inherited_t(parent)
{
    INC_THIS(false);
    // qRegisterMetaType for quee signals/slots between threads.
    qRegisterMetaType<QSharedPointer<model::Notes>>("QSharedPointer<model::Notes>");

    m_roles[NMR_Id] = "id";
    m_roles[NMR_Del] = "del";

    m_items = QSharedPointer<Notes>::create();
    m_storage = StorageFactory::instance().make("sqlite");
    if(m_storage && m_storage->isInit()) {
        connect(m_storage, SIGNAL(fireTaskProgress(nq::ProgressInfo,QVariant)), SLOT(onTaskProgress(nq::ProgressInfo,QVariant)));
        connect(m_storage, SIGNAL(fireTaskNoteProgress(nq::ProgressInfo,QSharedPointer<model::Notes>)), SLOT(onTaskNoteProgress(nq::ProgressInfo,QSharedPointer<model::Notes>)));
        m_storage->fetchNotes(10);
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
    if(item->asDel()) {
        LOG_TP(item->text());
    }
    //    const QString dt = QDateTime::fromMSecsSinceEpoch(item.msecs()).toString(QString("MM-dd hh:mm:ss.zzz"));
    const bool odd = indexRow % 2;

    switch (role) {
    case NMR_Id:
        return item->id();
    case NMR_TsCreated:
        return item->tsCreated();
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
    Notes *n = new Notes;
    LOG_TP(note->tsCreated());
    n->items.append(note);
    QSharedPointer<Notes> sp(n);
    m_storage->addNotes3(10, sp);
}

void NoteModel::markAsDeleted(qint32 id)
{
    QVector<qint32> ids;
    ids.append(id);
    m_storage->markNotesAsDeleted(20, ids);
}

void NoteModel::remove(qint32 id)
{

}

void NoteModel::onTaskProgress(const nq::ProgressInfo &pi, const QVariant &sp)
{
    if(pi.id==10 && pi.status == ProgressInfo::TPS_Success) {
        QSharedPointer<Notes> spnl = sp.value< QSharedPointer<Notes> >();
        beginResetModel();
        foreach (auto item, m_items->items) {
            delete item;
        }
        m_items->items.clear();
        Notes *notes = spnl.data();
        while(notes->items.count()){
            m_items->items.append(notes->items.takeFirst());
        }
        endResetModel();
    }else if(pi.id==20 && pi.status == ProgressInfo::TPS_Success){
        QSharedPointer<Notes> spnl = sp.value< QSharedPointer<Notes> >();
        beginResetModel();
        foreach (auto item, m_items->items) {
            delete item;
        }
        m_items->items.clear();
        Notes *notes = spnl.data();
        while(notes->items.count()){
            m_items->items.append(notes->items.takeFirst());
        }
        endResetModel();
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
    int sr = sortRole();
    QVariant leftData = left.data(NoteModel::NMR_TsCreated);
    QVariant rightData = right.data(NoteModel::NMR_TsCreated);
    int ldt = leftData.type();
    QString ldtn = leftData.typeName();

    if (leftData.type() == QVariant::LongLong) {
        return leftData.toLongLong() < rightData.toLongLong();
    } else {
        QString leftString = leftData.toString();
        QString rightString = rightData.toString();
        return QString::localeAwareCompare(leftString, rightString) < 0;
    }
    //    qDebug() << Q_FUNC_INFO
    //             << left.data(NoteModel::NMR_Debug).toString()
    //             << right.data(NoteModel::NMR_Debug).toString();
    //    return left.data().toString().compare(right.data().toString());
}
