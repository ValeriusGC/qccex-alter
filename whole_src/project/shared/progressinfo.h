#ifndef PROGRESSINFO_H
#define PROGRESSINFO_H

#include <QtCore>
#include <QDebug>

namespace nq {

/**
 *  Useful struct for pass information about progress.
 * @brief The ProgressInfo struct
 */
struct ProgressInfo
{
    enum Status_e{
        TPS_Progress,
        TPS_Success,
        TPS_Warning,
        TPS_Error,
        TPS_Cancelled
    };
    const qint64 id;
    Status_e status;
    qint32 min;
    qint32 max;
    qint32 pos;
    QString message;

    ProgressInfo(qint64 id=0) : id(id), status(TPS_Success), min(0), max(100), pos(100), message("")
    {
//        qDebug() << this << id << message;
    }
    ProgressInfo(qint64 id, Status_e status, qint32 min, qint32 max, qint32 pos, const QString &message)
        : id(id), status(status), min(min), max(max), pos(pos), message(message)
    {
//        qDebug() << this << id << message;
    }
//    ProgressInfo(ProgressInfo &pi);
//    ProgressInfo &operator=(const ProgressInfo &pi);
};

} // namespace nq

Q_DECLARE_METATYPE(nq::ProgressInfo)

#endif // PROGRESSINFO_H
