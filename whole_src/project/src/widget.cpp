#include <QPushButton>
#include <QVBoxLayout>
#include <QFrame>
#include <QHBoxLayout>
#include <QSignalMapper>
#include <QMessageBox>
#include <QDockWidget>
#include <QUndoStack>
#include <QUndoGroup>
#include <QMenuBar>
#include <QMenu>

#include "widget.h"
#include "sqlitestorage.h"
#include "notewidget.h"
#include "settings.h"
#include "version.h"
#include "shared/shared_def.h"

using namespace nq;
using namespace vfx_shared;
using namespace storage;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    // And this - from local folder
    setGeometry(Settings::instance().value("Geometry", QRect(100, 100, 600, 480)).toRect());

    init();
}

Widget::~Widget()
{
    // And this - from local folder
    Settings::instance().setValue("Geometry", geometry());
    // Without this we can see 'Please instantiate the QApplication object first' when destroed Meyers' singleton
    Settings::instance().sync();
}

void Widget::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
}

void Widget::buttonClicked(QWidget *sender)
{
    QPushButton *b = qobject_cast<QPushButton*>(sender);
    if(b != nullptr) {
        LOG_TP(b->objectName());
        Settings::instance().setApplicationLocale(b->objectName());
        retranslateUi();
        LOG_TP(Settings::instance().localeName());

        QMessageBox msgBox;
        msgBox.setText(tr("Done!"));
        msgBox.setStandardButtons(QMessageBox::Close);
        msgBox.exec();
    }
}

void Widget::fireInitProgress(const ProgressInfo &pi)
{
    Q_UNUSED(pi);
    //    LOG_TP(pi.message);
}

void Widget::add()
{

}

void Widget::setupUi()
{
    QVBoxLayout *mainVLayout = new QVBoxLayout(this);

    m_menuBar = new QMenuBar(this);
    mainVLayout->addWidget(m_menuBar);

    QFrame *buttonFrame = new QFrame(this);
    QFrame *notePanelFrame = new QFrame(this);
    mainVLayout->addWidget(buttonFrame);
    mainVLayout->addWidget(notePanelFrame);

    createMenuBar(m_menuBar);
    createTestButtons(buttonFrame);
    createNotePanel(notePanelFrame);
}

void Widget::retranslateUi()
{
    m_buttons[0]->setText(tr("to russian"));
    m_buttons[0]->setObjectName("ru");
    m_buttons[1]->setText(tr("to ukrainian"));
    m_buttons[1]->setObjectName("uk");
    m_buttons[2]->setText(tr("to default"));
    m_buttons[2]->setObjectName("");

    m_menuEdit->setTitle(tr("&Edit"));
    m_undoAction->setText(tr("Undo"));
    m_redoAction->setText(tr("Redo"));
}

void Widget::createMenuBar(QMenuBar *parent)
{
    m_menuEdit = parent->addMenu("");

    m_addNewAction = new QAction(tr("&New...", "UI"), this);
    m_addNewAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    m_menuEdit->addAction(m_addNewAction);
    m_delAction = new QAction(tr("&Delete", "UI"), this);
    m_delAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Delete));
    m_menuEdit->addAction(m_delAction);
    m_editAction = new QAction(tr("&Edit...", "UI"), this);
    m_editAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
    m_menuEdit->addAction(m_editAction);

    m_menuEdit->addSeparator();

    m_undoGroup = new QUndoGroup(this);
    m_undoAction = m_undoGroup->createUndoAction(this);
    m_undoAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z));
    m_undoAction->setObjectName("undoAction");
    m_redoAction = m_undoGroup->createRedoAction(this);
    m_redoAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Z));
    m_redoAction->setObjectName("redoAction");
    m_menuEdit->addAction(m_undoAction);
    m_menuEdit->addAction(m_redoAction);
}

void Widget::createTestButtons(QWidget *parent)
{
    QHBoxLayout *layout = new QHBoxLayout(parent);

    m_buttons.append(new QPushButton(this));
    m_buttons.append(new QPushButton(this));
    m_buttons.append(new QPushButton(this));

    m_signalMapper = new QSignalMapper(this);

    for (auto b : m_buttons) {
        connect(b, SIGNAL(clicked()), m_signalMapper, SLOT(map()));
        m_signalMapper->setMapping(b, b);
        layout->addWidget(b);
    }
    connect(m_signalMapper, SIGNAL(mapped(QWidget*)), SLOT(buttonClicked(QWidget*)));
}

void Widget::createNotePanel(QWidget *parent)
{
    //    QDockWidget *dockLog = new QDockWidget(this);
    //    dockLog->setObjectName("dock_log");
    //    dockLog->setAllowedAreas(Qt::BottomDockWidgetArea);
    //    addDockWidget(Qt::BottomDockWidgetArea, dockLog);
    QVBoxLayout *layout = new QVBoxLayout(parent);

    NoteWidget *nw = new NoteWidget(this);
    layout->addWidget(nw);
    m_undoGroup->setActiveStack(nw->undoStack());
    connect(m_addNewAction, SIGNAL(triggered()), nw, SLOT(addNote()));
    connect(m_editAction, SIGNAL(triggered()), nw, SLOT(editNote()));
    connect(m_delAction, SIGNAL(triggered()), nw, SLOT(delNote()));

    nw->setFocus();
}

void Widget::init()
{
    StorageObjectOperations *storage = &SqliteStorage::instance();
    connect(storage, SIGNAL(fireInitProgress(nq::ProgressInfo)), SLOT(fireInitProgress(nq::ProgressInfo)));
    connect(storage, SIGNAL(fireNotesAdded(QVector<qint32>)), SLOT(onNotesAdded(QVector<qint32>)));
    connect(storage, SIGNAL(fireNotesMarkedAsDeleted(QVector<qint32>)), SLOT(onNotesMarkedAsDeleted(QVector<qint32>)));
    storage->init("qccex.db");

    setupUi();
    retranslateUi();
}
