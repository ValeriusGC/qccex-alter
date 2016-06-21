#include <QTextEdit>
#include <QDialogButtonBox>
//#include <QPushButton>
#include <QVBoxLayout>
//#include <QHBoxLayout>

#include "newnotedialog.h"
#include "shared_def.h"
#include "sqlitestorage.h"

using namespace nq;
using namespace vfx_shared;
using namespace storage;

#define TASK_ID_EDIT_NOTE   TASK_ID_USER+1

NewNoteDialog::NewNoteDialog(const QString &uuid, QWidget *parent) :
    Inherited_t(parent),
    m_uuid(uuid),
    m_new(false),
    m_oldNote(nullptr),
    m_editedNote(nullptr)
{
    INC_THIS(false);
    m_textEdit = new QTextEdit(this);
    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    //    m_buttonAdd = new QPushButton(tr("Add", "Widget"), this);
    //    m_buttonCancel = new QPushButton(tr("Cancel", "Widget"), this);
    //    QLayout* hl = new QHBoxLayout;
    //    hl->addWidget(m_buttonCancel);
    //    hl->addWidget(m_buttonAdd);
    QVBoxLayout* vl = new QVBoxLayout;
    vl->addWidget(m_textEdit);
    vl->addWidget(m_buttonBox);
    //    vl->addLayout(hl);
    setLayout( vl );

    connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    m_new = m_uuid.isEmpty();
    if(!m_new) {
        SqliteStorage *storage = &SqliteStorage::instance();
        if(storage->isInit()) {
            connect(storage, SIGNAL(fireTaskProgress(nq::ProgressInfo,QVariant)),
                    SLOT(onTaskProgress(nq::ProgressInfo,QVariant)));

            UuidVector_t ids;
            ids.append(m_uuid);
            storage->fetchNotes(TASK_ID_EDIT_NOTE, ids);
        }
    }
}

NewNoteDialog::~NewNoteDialog()
{
    delete m_editedNote;
    delete m_oldNote;
    DEC_THIS(false);
}
QTextEdit *NewNoteDialog::textEdit() const
{
    return m_textEdit;
}

model::Note *NewNoteDialog::oldNote() const
{
    return m_oldNote;
}

model::Note *NewNoteDialog::editedNote() const
{
    return m_editedNote;
}

void NewNoteDialog::onTaskProgress(const ProgressInfo &pi, const QVariant &sp)
{
    if(pi.status==ProgressInfo::TPS_Success && pi.id == TASK_ID_EDIT_NOTE) {
        QSharedPointer<Notes> spnl = sp.value< QSharedPointer<Notes> >();
        Notes *notes = spnl.data();
        if(notes->items.count()){
            m_oldNote = notes->items.at(0)->copy();
            m_editedNote = notes->items.at(0)->copy();
            m_textEdit->setText(m_oldNote->text());
        }
    }
}

