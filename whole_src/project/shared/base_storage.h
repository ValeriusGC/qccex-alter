#ifndef BASESTORAGE_H
#define BASESTORAGE_H

#include <QObject>
#include <QSharedPointer>

//#include "sb_def.h"

#include "storage_globals.h"
using namespace storage;

#include "basemodelitemlist.h"

//#include "note.h"

//namespace model {
//QT_FORWARD_DECLARE_CLASS(NoteList)
//}


/**
 *  Useful struct for pass information about progress.
 * @brief The ProgressInfo struct
 */
struct ProgressInfo {
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

    ProgressInfo(qint64 id=0) : id(id), status(TPS_Success), min(0), max(100), pos(100), message("") {
    }

    ProgressInfo(qint64 id, Status_e status, qint32 min, qint32 max, qint32 pos, const QString &message)
        : id(id), status(status), min(min), max(max), pos(pos), message(message) {
    }

};

Q_DECLARE_METATYPE(ProgressInfo)

///**
// *
// * @brief The Task class
// */
//class Task : public QObject{
//    Q_OBJECT
//public:
//    explicit Task(QObject *parent) : QObject(parent) {
//        INC_THIS(true);
//    }
//    virtual ~Task() {
//        DEC_THIS(true);
//    }

////    void progress(qint32 inc) {
////        const qint32 newPos = pos + inc;
////        if(newPos < ma)
////        if()
////    }

//private:
//    qint64 m_id;
//    QObject *m_value;

//};

class BaseStorage : public QObject
{
    Q_OBJECT
public:
    explicit BaseStorage(QObject *parent = 0);
    ~BaseStorage();

    QString name() const;

    void init(const QString &name) Q_DECL_FINAL;

    bool isInit() const Q_DECL_FINAL;

    /**
     *  Returns version, that is stored in.
     * @brief version
     * @return current version
     */
    BoolVariantResult_t version() const Q_DECL_FINAL;

    void fetchNotes(qint64 id) Q_DECL_FINAL;
    void clearNotes(qint64 id) Q_DECL_FINAL;
    void addNotes(qint64 id, const QSharedPointer<QObject> &notes) Q_DECL_FINAL;

    void fetchAuthors(qint64 id) Q_DECL_FINAL;

protected:

    /**
     * Used internal by descendants in init() and doCheckVersion();
     * @brief ACTUAL_VERSION
     */
    static const qint32 NEW_VERSION;

    /**
     *  Creates storage items
     * @brief doCreateItems
     */
    virtual void doCreateItems() = 0;

    /**
     *  Checks current version of storage.
     * @brief doCheckVersion
     * @return Current version or NO_VERSION with info-string.
     */
    virtual BoolVariantResult_t doCheckVersion() const = 0;

    virtual BoolResult_t doCreate() = 0;

    virtual BoolResult_t doUpgrade(qint32 oldVersion) = 0;

    virtual void doFetchNotes(qint64 id) = 0;
    virtual void doClearNotes(qint64 id) = 0;
    virtual void doAddNotes(qint64 id, const QSharedPointer<QObject> &notes) = 0;

    virtual void doFetchAuthors(qint64 id) = 0;

signals:
    void fireInitProgress(const ProgressInfo &pi);
    void fireTaskProgress(const ProgressInfo &pi, const QVariant &sp);
//    void fireTaskProgress(const ProgressInfo &pi, const QSharedPointer<QObject> &sp);

public slots:

private:
    QString m_name;
    bool m_isInit;
};

#endif // BASESTORAGE_H
