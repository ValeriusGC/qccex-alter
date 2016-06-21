#ifndef NEWNOTEDIALOG_H
#define NEWNOTEDIALOG_H

#include <QDialog>

#include "progressinfo.h"

QT_BEGIN_NAMESPACE
class QTextEdit;
class QPushButton;
class QDialogButtonBox;
QT_END_NAMESPACE

namespace model {
    QT_FORWARD_DECLARE_CLASS(Note)
}

namespace nq {


class NewNoteDialog : public QDialog
{
    Q_OBJECT
    typedef QDialog Inherited_t;
public:
    explicit NewNoteDialog(const QString &uuid, QWidget *parent = 0);
    ~NewNoteDialog();

    QTextEdit *textEdit() const;

    model::Note *oldNote() const;
    model::Note *editedNote() const;

signals:

public slots:

private slots:
    void onTaskProgress(const nq::ProgressInfo &pi, const QVariant &sp);

private:
    QString m_uuid;
    bool m_new;
    model::Note *m_oldNote;
    model::Note *m_editedNote;
    QTextEdit *m_textEdit;
    QDialogButtonBox *m_buttonBox;
};

} // namespace nq

#endif // NEWNOTEDIALOG_H
