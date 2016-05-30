#include <QPushButton>
#include <QVBoxLayout>
#include <QFrame>
#include <QHBoxLayout>
#include <QSignalMapper>
#include <QMessageBox>
#include <QDockWidget>

#include "widget.h"
#include "notewidget.h"
#include "storagefactory.h"
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
    LOG_TP(pi.message);
}

void Widget::setupUi()
{
    QVBoxLayout *mainVLayout = new QVBoxLayout(this);
    QFrame *buttonFrame = new QFrame(this);
    QFrame *notePanelFrame = new QFrame(this);
    mainVLayout->addWidget(buttonFrame);
    mainVLayout->addWidget(notePanelFrame);

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
    nw->setFocus();
}

void Widget::init()
{
    BaseStorage *storage = StorageFactory::instance().make("sqlite");
    if(storage != nullptr){
        connect(storage, SIGNAL(fireInitProgress(ProgressInfo)), SLOT(fireInitProgress(ProgressInfo)));
        storage->init("qccex.db");
    }

    setupUi();
    retranslateUi();
}
