#ifndef NEWNOTEDIALOG_H
#define NEWNOTEDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QTextEdit;
class QPushButton;
class QDialogButtonBox;
QT_END_NAMESPACE


namespace nq {


class NewNoteDialog : public QDialog
{
    Q_OBJECT
    typedef QDialog Inherited_t;
public:
    explicit NewNoteDialog(QWidget *parent = 0);
    ~NewNoteDialog();

    QTextEdit *textEdit() const;

signals:

public slots:

private:
    QTextEdit *m_textEdit;
    //    QPushButton *m_buttonAdd;
//    QPushButton *m_buttonCancel;
    QDialogButtonBox *m_buttonBox;
};

} // namespace nq

#endif // NEWNOTEDIALOG_H
