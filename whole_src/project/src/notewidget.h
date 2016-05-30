#ifndef NOTEWIDGET_H
#define NOTEWIDGET_H

#include <QFrame>

#include "notemodel.h"
//namespace model {
//QT_FORWARD_DECLARE_CLASS(NoteModel)
//}

QT_BEGIN_NAMESPACE
class QListView;
class QListWidgetItem;
class QLabel;
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE

//QT_FORWARD_DECLARE_CLASS(Settings)
//QT_FORWARD_DECLARE_CLASS(LeftPaneCtrl)
//QT_FORWARD_DECLARE_CLASS(ModeCtrl)
//QT_FORWARD_DECLARE_CLASS(GroupWindow)


namespace nq {

QT_FORWARD_DECLARE_CLASS(NewNoteWidget)

class NoteWidget : public QFrame
{
    Q_OBJECT
    typedef QFrame Inherited_t;
public:
    explicit NoteWidget(QWidget *parent = 0);
    ~NoteWidget();

protected:
    void focusInEvent(QFocusEvent * event);

signals:

public slots:
    void newNoteClicked();

private slots:
    void onBtnClicked();
    void onItemActivated(QListWidgetItem *item);
    void onCurrentItemChanged(QListWidgetItem * current, QListWidgetItem * previous);

private:
    NewNoteWidget *m_newNoteWidget;
    QListView *m_listWidget;
    model::NoteModel *m_model;

//    void makeItem(QListWidget *listWidget);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief The OneNote class
 */
class OneNote : public QWidget {
    Q_OBJECT
    typedef QWidget Inherited_t;
public:
    explicit OneNote(QWidget *parent = 0);
    ~OneNote();

    QLabel *labelCreated() const;
    QLabel *labelEdited() const;
    QLabel *labelText() const;

signals:

public slots:

private slots:

private:
    QLabel *m_labelCreated;
    QLabel *m_labelEdited;
    QLabel *m_labelText;
};

// ~The OneNote class

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class NewNoteWidget : public QWidget {
    Q_OBJECT
    typedef QWidget Inherited_t;
public:
    explicit NewNoteWidget(QWidget *parent = 0);
    ~NewNoteWidget();

    QLineEdit *editSearch() const;
    QPushButton *buttonInsert() const;

protected:
    void focusInEvent(QFocusEvent * event);


signals:

public slots:

private slots:

private:
    QLineEdit *m_editSearch;
    QPushButton *m_buttonInsert;
};


} // namespace nq

Q_DECLARE_METATYPE(nq::OneNote*)


#endif // NOTEWIDGET_H
