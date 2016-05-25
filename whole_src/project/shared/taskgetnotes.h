#ifndef TASKGETNOTES_H
#define TASKGETNOTES_H

#include <QObject>

class TaskGetNotes : public QObject
{
    Q_OBJECT
public:
    explicit TaskGetNotes(QObject *parent = 0);
    ~TaskGetNotes();

signals:

public slots:

private:
    qint64 m_id;

};

#endif // TASKGETNOTES_H
