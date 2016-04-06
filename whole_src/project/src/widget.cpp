#include <QPushButton>
#include <QHBoxLayout>
#include <QSignalMapper>
#include <QMessageBox>

#include "widget.h"
#include "settings.h"
#include "note_defines.h"
#include "version.h"

using namespace nq;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    // And this - from local folder
    setGeometry(Settings::instance().value("Geometry", QRect(100, 100, 600, 480)).toRect());

    // Main widget
    // Main horizontal layout automatically set to #vertWidget
    QHBoxLayout *mainHLayout = new QHBoxLayout(this);

    m_buttons.append(new QPushButton(this));
    m_buttons.append(new QPushButton(this));
    m_buttons.append(new QPushButton(this));

    m_signalMapper = new QSignalMapper(this);

    for (auto b : m_buttons) {
        connect(b, SIGNAL(clicked()), m_signalMapper, SLOT(map()));
        m_signalMapper->setMapping(b, b);
        mainHLayout->addWidget(b);
    }
    connect(m_signalMapper, SIGNAL(mapped(QWidget*)), SLOT(buttonClicked(QWidget*)));

    retranslateUi();
}

Widget::~Widget()
{
    // And this - from local folder
    Settings::instance().setValue("Geometry", geometry());
    // Without this we can see 'Please instantiate the QApplication object first' when destroed Meyers' singleton
    Settings::instance().sync();
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

void Widget::retranslateUi()
{
    m_buttons[0]->setText(tr("to russian"));
    m_buttons[0]->setObjectName("ru");
    m_buttons[1]->setText(tr("to ukrainian"));
    m_buttons[1]->setObjectName("uk");
    m_buttons[2]->setText(tr("to default"));
    m_buttons[2]->setObjectName("");
}
