#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include "progressinfo.h"
using namespace nq;

QT_BEGIN_NAMESPACE
class QSignalMapper;
class QPushButton;
QT_END_NAMESPACE


class Widget : public QWidget
{
    Q_OBJECT

public:

    Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void focusInEvent(QFocusEvent * event);

private slots:
    void buttonClicked(QWidget *sender);
    void fireInitProgress(const nq::ProgressInfo &pi);


private:
    QSignalMapper *m_signalMapper;

    QVector<QPushButton*> m_buttons;

    void setupUi();

    void retranslateUi();

    void createTestButtons(QWidget *parent);

    void createNotePanel(QWidget *parent);

    void init();

};

#endif // WIDGET_H
