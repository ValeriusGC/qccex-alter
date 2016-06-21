#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include "progressinfo.h"
using namespace nq;

QT_BEGIN_NAMESPACE
class QSignalMapper;
class QPushButton;
class QUndoStack;
class QMenuBar;
class QMenu;
class QUndoGroup;
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

    void add();


private:
    QSignalMapper *m_signalMapper;
    QVector<QPushButton*> m_buttons;

    QMenuBar *m_menuBar;
    QMenu *m_menuEdit;
    QUndoGroup *m_undoGroup;

    QAction *m_undoAction;
    QAction *m_redoAction;

    QAction *m_addNewAction;
    QAction *m_delAction;
    QAction *m_editAction;


    void setupUi();

    void retranslateUi();

    void createMenuBar(QMenuBar *parent);
    void createTestButtons(QWidget *parent);
    void createNotePanel(QWidget *parent);

    void init();

};

#endif // WIDGET_H
