#include <QTextEdit>
#include <QDialogButtonBox>
//#include <QPushButton>
#include <QVBoxLayout>
//#include <QHBoxLayout>

#include "newnotedialog.h"
#include "shared_def.h"

using namespace nq;
using namespace vfx_shared;

NewNoteDialog::NewNoteDialog(QWidget *parent) : Inherited_t(parent)
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
}

NewNoteDialog::~NewNoteDialog()
{
    DEC_THIS(false);
}
QTextEdit *NewNoteDialog::textEdit() const
{
    return m_textEdit;
}

