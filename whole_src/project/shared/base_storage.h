#ifndef BASESTORAGE_H
#define BASESTORAGE_H

#include <QObject>
#include <QSharedPointer>

//#include "sb_def.h"

#include "storage_globals.h"
//using namespace storage;

#include "basemodelitemlist.h"

#include "progressinfo.h"

#include "note.h"

namespace storage {

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
//    void addNotes(qint64 id, const QSharedPointer<QObject> &notes) Q_DECL_FINAL;
//    void addNotes2(qint64 id, const QVector<QSharedPointer<model::Note>> &notes) Q_DECL_FINAL;
    void addNotes3(qint64 id, const QSharedPointer<model::Notes> &notes) Q_DECL_FINAL;
    void markNotesAsDeleted(qint64 id, const QVector<qint32> &ids);

    void removeNotes(qint64 id, const QVector<qint32> &ids) Q_DECL_FINAL;

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
//    virtual void doAddNotes(qint64 id, const QSharedPointer<QObject> &notes) = 0;
//    virtual void doAddNotes2(qint64 id, const QVector<QSharedPointer<model::Note>> &notes) = 0;
    virtual void doAddNotes3(qint64 id, const QSharedPointer<model::Notes> &notes) = 0;

    virtual void doMarkNotesAsDeleted(qint64 id, const QVector<qint32> &ids) = 0;

    virtual void doRemoveNotes(qint64 id, const QVector<qint32> &ids) = 0;

    virtual void doFetchAuthors(qint64 id) = 0;


signals:
    void fireInitProgress(const nq::ProgressInfo &pi);
    void fireTaskProgress(const nq::ProgressInfo &pi, const QVariant &sp);
    void fireTaskNoteProgress(const nq::ProgressInfo &pi, const QSharedPointer<model::Notes> &notes);
//    void fireTaskProgress(const ProgressInfo &pi, const QSharedPointer<QObject> &sp);

public slots:

private:
    QString m_name;
    bool m_isInit;
};

} // namespace storage

#endif // BASESTORAGE_H
