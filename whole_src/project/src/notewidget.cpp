#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QLabel>

#include "notewidget.h"
#include "shared_def.h"
#include "newnotedialog.h"
#include "noteitemdelegate.h"

//#include "notemodel.h"
using namespace vfx_shared;
using namespace model;
using namespace nq;

NoteWidget::NoteWidget(QWidget *parent) : Inherited_t(parent)
{
    INC_THIS(true);
    setFrameStyle(QFrame::StyledPanel | QFrame::Plain);

    m_model = new NoteModel(this);
    QSortFilterProxyModel *proxyModel = new MySortFilterProxyModel(this);
    proxyModel->setSourceModel(m_model);
    proxyModel->sort(0, Qt::DescendingOrder);

    m_newNoteWidget = new NewNoteWidget(this);
    m_newNoteWidget->editSearch()->setPlaceholderText(tr("Search string (NUY)", "Widgets"));
    m_newNoteWidget->buttonInsert()->setText("+");

    m_listWidget = new QListView(this);
    m_listWidget->setModel(proxyModel);
    //NoteItemDelegate *delegate = new NoteItemDelegate(this);
    QFile f(QString(":/templates/NoteShowingTemplate.html"));
    if (f.open(QFile::ReadOnly)){
        m_template = f.readAll();
    }
    m_listWidget->setItemDelegate(new NoteItemDelegate(m_template, this));


    QLayout* l = new QVBoxLayout;
    l->addWidget( m_newNoteWidget );
    l->addWidget( m_listWidget );
    setLayout( l );

    connect(m_listWidget, SIGNAL(doubleClicked(QModelIndex)), SLOT(onDblClicked(QModelIndex)));


//    connect(m_listWidget, SIGNAL(itemActivated(QListWidgetItem*)), SLOT(onItemActivated(QListWidgetItem*)));
    connect(m_newNoteWidget->buttonInsert(), SIGNAL(clicked()), SLOT(newNoteClicked()));
//    connect(m_listWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
//            SLOT(onCurrentItemChanged(QListWidgetItem*,QListWidgetItem*)));

    // Добавим в список 10 элементов
//    for( int i = 0; i < 100; ++i ) {
//        makeItem( m_listWidget );
//    }

    // commented 16/05/28
    //    QHBoxLayout *lay = new QHBoxLayout;
    //    setLayout(lay);

    //    QListView *logTableView = new QListView(this);
    //    logTableView->setModel(m_model);
    //    lay->addWidget(logTableView);
    //    // Makes automatically scrolling to bottom
    //    connect(m_model, SIGNAL(rowsInserted(QModelIndeQStringx,int,int)), logTableView, SLOT(scrollToBottom()));
    // ~commented 16/05/28

    //    Settings set;
    //    int h = set.value("Logger/Height", height()).toInt();
    //    resize(width(), h);

    //    // Initially scroll to bottom
    //    logTableView->scrollToBottom();
}

NoteWidget::~NoteWidget()
{
    DEC_THIS(true);
}

void NoteWidget::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    m_listWidget->setFocus();
    if(m_listWidget->model()->rowCount()) {
        m_listWidget->setCurrentIndex(m_listWidget->model()->index(0, 0));
    }
}

void NoteWidget::newNoteClicked()
{
    QScopedPointer<NewNoteDialog> dlg(new NewNoteDialog);
    if (dlg->exec() == QDialog::Accepted){
        LOG_TP(dlg->textEdit()->toPlainText());
        Note *note = new Note;
        note->setText(dlg->textEdit()->toPlainText());
        note->setAsDel(0);
        m_model->add(note);
        setFocus();
    }
}

void NoteWidget::onBtnClicked()
{
    if( QPushButton* btn = qobject_cast< QPushButton* >( sender() ) ) {
        if( QLineEdit* e = btn->parent()->findChild< QLineEdit* >() ) {
            QMessageBox::information( this, "Button was clicked!", e->text() );
            return;
        }
    }
}

void NoteWidget::onItemActivated(QListWidgetItem *item)
{
    QVariant v = item->data(0);
    OneNote *btn = v.value<OneNote *>();

    LOG;

    if(btn) {
        if( QLineEdit* e = btn->parent()->findChild< QLineEdit* >() ) {
            QMessageBox::information( this, "Button was clicked!", e->text() );
            return;
        }
    }

    // What called in roles:
//    if( QLineEdit* e = item->findChild< QLineEdit* >() ) {
//        QMessageBox::information( this, "activated!", e->text() );
//        return;
    //    }
}

void NoteWidget::onCurrentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(previous);
    QVariant v = current->data(0);
    OneNote *btn = v.value<OneNote *>();
    if(btn) {
        btn->labelText()->setText("here");
    }
}

void NoteWidget::onDblClicked(const QModelIndex &index)
{
    qint32 id = index.data(NoteModel::NMR_Id).toInt();
    m_model->markAsDeleted(id);
    LOG_TP(id);
}

//void NoteWidget::makeItem(QListWidget *listWidget)
//{
////    QWidget* wgt = new QWidget;
////    QLayout* l = new QHBoxLayout;
////    l->addWidget( new QLineEdit );
////    QPushButton* btn = new QPushButton( "Click me" );
////    connect( btn, SIGNAL( clicked() ), SLOT( onBtnClicked() ) );
////    l->addWidget( btn );
////    wgt->setLayout( l );

//    OneNote *on = new OneNote(this);
//    QListWidgetItem* item = new QListWidgetItem( listWidget );
//    item->setSizeHint( on->sizeHint() );
//    listWidget->setItemWidget( item, on );
//    item->setData(0, QVariant::fromValue(on));
//}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

OneNote::OneNote(QWidget *parent) : Inherited_t(parent)
{
    INC_THIS(false);
    QLayout* tsL = new QHBoxLayout;
    m_labelCreated = new QLabel("m_labelCreated", this);
    m_labelEdited = new QLabel("m_labelEdited", this);
    tsL->addWidget(m_labelCreated);
    tsL->addWidget(m_labelEdited);
    QVBoxLayout* tsV = new QVBoxLayout;
    m_labelText = new QLabel("m_labelText", this);
    tsV->addLayout(tsL);
    tsV->addWidget(m_labelText);

    this->setLayout(tsV);
}

OneNote::~OneNote()
{
    DEC_THIS(false);
}

QLabel *OneNote::labelCreated() const
{
    return m_labelCreated;
}

QLabel *OneNote::labelEdited() const
{
    return m_labelCreated;
}

QLabel *OneNote::labelText() const
{
    return m_labelText;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

NewNoteWidget::NewNoteWidget(QWidget *parent) : Inherited_t(parent)
{
    INC_THIS(false);
    QLayout* tsL = new QHBoxLayout;
    m_editSearch = new QLineEdit(this);
    m_buttonInsert = new QPushButton(this);
    tsL->addWidget(m_editSearch);
    tsL->addWidget(m_buttonInsert);
    this->setLayout(tsL);

}

NewNoteWidget::~NewNoteWidget()
{
    DEC_THIS(false);
}

QLineEdit *NewNoteWidget::editSearch() const
{
    return m_editSearch;
}

QPushButton *NewNoteWidget::buttonInsert() const
{
    return m_buttonInsert;
}

void NewNoteWidget::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    LOG;
}

