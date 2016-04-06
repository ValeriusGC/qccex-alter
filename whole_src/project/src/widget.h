#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

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

private slots:
    void buttonClicked(QWidget *sender);

private:
    QSignalMapper *m_signalMapper;

    QVector<QPushButton*> m_buttons;

    void retranslateUi();
};

#endif // WIDGET_H
