#include <QSqlQuery>
#include <QSqlDriver>

#include "sqlitestorage.h"
#include "storage_globals.h"
#include "note.h"
#include "notemodel.h"
#include "authormodel.h"
#include "shared_const.h"

// Include all old versions to make upgrade
#include "sqlite_storage_v1.h"
#include "sqlite_storage_v2.h"
// Include fresh version
#include "sqlitestorage_elements.h"

// make direct access to current version namespace.
using namespace storage;
using namespace storage::sqlite;


qint32 DbHelper::m_counter = 0;

SqliteStorage::SqliteStorage(QObject *parent) : Inhherited_t(parent)
{
    m_tableMap[TableCfg::TBL_NAME] = StoragePtr_t(new TableCfg);
    m_tableMap[TableNote::TBL_NAME] = StoragePtr_t(new TableNote);
    m_tableMap[TableAuthor::TBL_NAME] = StoragePtr_t(new TableAuthor);
    m_tableMap[TableTags::TBL_NAME] = StoragePtr_t(new TableTags);
    m_tableMap[TableTagsNotes::TBL_NAME] = StoragePtr_t(new TableTagsNotes);
}

SqliteStorage::~SqliteStorage()
{

}

BoolVariantResult_t SqliteStorage::tables() const
{
    DbHelper dbh(name());
//    QSqlDatabase db = dbh.db();
    if(!dbh.result().result()){
        return {false, dbh.result().data()};
    }

    return {true, dbh.db().tables()};
}

BoolVariantResult_t SqliteStorage::doCreate(QSqlDatabase db)
{
    Q_UNUSED(db);
    return {false, QVariant()};
}

BoolVariantResult_t SqliteStorage::doUpgrade(QSqlDatabase db, const V1_t &dummy)
{
    // if ok
    {
        const QString strFindDefAuthorId =
                QString(QStringLiteral("SELECT %1 FROM %2 WHERE %3 = '%4'")
                        .arg(TableAuthor::FLD_ID).arg(TableAuthor::TBL_NAME)
                        .arg(TableAuthor::FLD_TITLE).arg(Constants::DEFAULT_AUTHOR));
        QSqlQuery q(db);
        if (!q.exec(strFindDefAuthorId)){
            return {false, q.lastError().text()};
        }else{
            if(q.next()){
                qint32 authId = q.value(0).toInt();
                const QString updateAuth =
                        QString(QStringLiteral("UPDATE %1 SET %2=%3 WHERE %4 is NULL"))
                        .arg(TableNote::TBL_NAME)
                        .arg(TableNote::FLD_FK_AUTHOR).arg(authId)
                        .arg(TableNote::FLD_FK_AUTHOR);
                if (!q.exec(updateAuth)){
                    return {false, q.lastError().text()};
                }
            }else{
                return {false, Q_FUNC_INFO};
            }
        }
    }

    {
        // Check if Note hasn't any tag - add default one.
        // SELECT tbl_notes.id FROM tbl_notes LEFT OUTER JOIN tbl_tags_notes ON tbl_notes.id = fk_notes WHERE fk_notes IS NULL
        const QString strFindOrphanNotes =
                QString(QStringLiteral("SELECT %1.%2 FROM %1 LEFT OUTER JOIN %3 ON %1.%2 = %4 WHERE %4 IS NULL")
                        .arg(TableNote::TBL_NAME).arg(TableNote::FLD_ID)
                        .arg(TableTagsNotes::TBL_NAME).arg(TableTagsNotes::FLD_FK_NOTES));

        QSqlQuery q(db);
        if (!q.exec(strFindOrphanNotes)){
            return {false, q.lastError().text()};
        }

        QSqlQuery qInsert(db);
        const QString QRY_INSERT(
                    QStringLiteral("insert into %1(%2, %3) values(?, ?)")
                    .arg(TableTagsNotes::TBL_NAME).arg(TableTagsNotes::FLD_FK_NOTES).arg(TableTagsNotes::FLD_FK_TAGS));
        if (!qInsert.prepare(QRY_INSERT)){
            return {false, qInsert.lastError().text()};
        }
        while (q.next()) {
            qInsert.addBindValue(q.value(TableNote::FLD_ID));
            qInsert.addBindValue(1);
            if (!qInsert.exec()){
                return {false, qInsert.lastError().text()};
            }
        }

    }

    return {true, dummy.value};
}

BoolVariantResult_t SqliteStorage::doUpgrade(QSqlDatabase db, const V2_t &dummy)
{
    qint32 tagId = 0;
    const QString strIncomingTagId =
            QString(QStringLiteral("SELECT %1 FROM %2 WHERE %3 = '%4'")
                    .arg(TableTags::FLD_ID).arg(TableTags::TBL_NAME)
                    .arg(TableTags::FLD_TEXT).arg(Constants::TAG_INCOMING));
    QSqlQuery q(db);
    if (!q.exec(strIncomingTagId)){
        return {false, q.lastError().text()};
    }else{
        if(q.next()){
            tagId = q.value(0).toInt();
        }else{
            return {false, Q_FUNC_INFO};
        }
    }

    // Check if Note hasn't any tag - add default one.
    // SELECT tbl_notes.id FROM tbl_notes LEFT OUTER JOIN tbl_tags_notes ON tbl_notes.id = fk_notes WHERE fk_notes IS NULL
    const QString strFindOrphanNotes =
            QString(QStringLiteral("SELECT %1.%2 FROM %1 LEFT OUTER JOIN %3 ON %1.%2 = %4 WHERE %4 IS NULL")
                    .arg(TableNote::TBL_NAME).arg(TableNote::FLD_ID)
                    .arg(TableTagsNotes::TBL_NAME).arg(TableTagsNotes::FLD_FK_NOTES));

    if (!q.exec(strFindOrphanNotes)){
        return {false, q.lastError().text()};
    }

    QSqlQuery qInsert(db);
    const QString QRY_INSERT(
                QStringLiteral("insert into %1(%2, %3) values(?, ?)")
                .arg(TableTagsNotes::TBL_NAME).arg(TableTagsNotes::FLD_FK_NOTES).arg(TableTagsNotes::FLD_FK_TAGS));
    if (!qInsert.prepare(QRY_INSERT)){
        return {false, qInsert.lastError().text()};
    }
    while (q.next()) {
        qInsert.addBindValue(q.value(TableNote::FLD_ID));
        qInsert.addBindValue(tagId);
        if (!qInsert.exec()){
            return {false, qInsert.lastError().text()};
        }
    }

    return {true, dummy.value};
}

//AuthorListResult_t SqliteStorage::getAuthors() const
//{
//    DbHelper dbh(name());
//    QSqlDatabase db = dbh.db();
//    if(!db.isOpen()){
//        return {nullptr, dbh.lastError()};
//    }

//    QSqlQuery q(db);
//    q.prepare(QString("SELECT * FROM %1").arg(TableAuthor::TBL_NAME));
//    if(!q.exec()) {
//        return {nullptr, q.lastError().text()};
//    }

//    model::AuthorList *items = new model::AuthorList;
//    while (q.next()) {
//        Author *item = new Author;
//        item->setId(q.value(TableAuthor::FLD_ID).toInt());
//        item->setTitle(q.value(TableAuthor::FLD_TITLE).toString());
//        items->data().append(item);
//    }
//    return {items, ""};
//}

void SqliteStorage::doCreateItems()
{
    //    BaseTable *table = new TableNote(this);
    //    m_items.insert(table->name(), table);
    //    table = new TableCfg(this);
    //    m_items.insert(table->name(), table);
}

BoolVariantResult_t SqliteStorage::doCheckVersion() const
{
    DbHelper dbh(name());
    if(!dbh.result().result()){
        return {false, dbh.result().data()};
    }
    // If all ok at start point
    QSqlDatabase db = dbh.db();

    // Check row count
    {
        QSqlQuery q(db);
        q.prepare(QString("SELECT COUNT (*) FROM tbl_config WHERE the_key = 'version'"));
        bool isExec = q.exec();
        if(!isExec) {
            return {false, q.lastError().text()};
        }

        qint32 rows;
        if (q.next()) {
            rows = q.value(0).toInt();
        }else{
            return {false, tr("No rows in query", "DB")};
        }

        //
        if(rows > 1) {
            return {false, tr("More than one record '%1'", "DB").arg("key=version")};
        }else if(rows == 0){
            return {false , tr("User: no such record '%1'", "DB").arg("key=version")};
        }else{
            // ALL IS OK;
            // version = rows;
        }
    }

    qint32 version = STORAGE_NO_EXISTS;
    {
        QSqlQuery q(db);
        q.prepare(QString("SELECT the_value FROM tbl_config WHERE the_key = 'version'"));
        bool isExec = q.exec();
        if(!isExec) {
            return {false, q.lastError().text()};
        }

        if (q.next()) {
            version = q.value("the_value").toInt();
        }else{
            return {false, tr("No rows in query", "DB")};
        }
    }
    return {true, version};
}

vfx_shared::BoolResult_t SqliteStorage::doCreate()
{
    //------------------------------------------------------------------------------------------------------------------
    // Transaction opens in DbHelper
    DbHelper dbh(name());
    if(!dbh.result().result()){
        return {false, dbh.result().data().toString()};
    }
    // If all ok at start point
    QSqlDatabase db = dbh.db();

    BoolResult_t res(true, "");
    bool f = db.driver()->hasFeature(QSqlDriver::LastInsertId);
    if(!f) {
        return {false, "No feature 'QSqlDriver::LastInsertId'"};
    }

    for(auto table: m_tableMap.values()){
        BoolVariantResult_t res = table->create(db);
        if(!res.result()) {
            return {false, res.data().toString()};
        }
    }

    // if ok
    {
        // 'Config' table and data
//        BoolResult_t created = TableCfg().init(db, TableNote::INVALID_VERSION);
//        if (!created.result()){
//            return {created.result(), created.data()};
//        }
    }

    {
        // 'Author' table and data
//        BoolResult_t created = TableAuthor().init(db, TableAuthor::INVALID_VERSION);
//        if (!created.result()){
//            return {created.result(), created.data()};
//        }
    }

    {
//        // Table Note
//        BoolResult_t created = TableNote().init(db, TableNote::INVALID_VERSION);
//        if (!created.result()){
//            return {created.result(), created.data()};
//        }
    }

    {
        // 'Tag' table and default data
//        BoolResult_t created = TableTags().init(db, TableTags::INVALID_VERSION);
//        if (!created.result()){
//            return {created.result(), created.data()};
//        }
    }

    {
        // 'TagsNotes' table
//        BoolResult_t created = TableTagsNotes().init(db, TableTagsNotes::INVALID_VERSION);
//        if (!created.result()){
//            return {created.result(), created.data()};
//        }
    }

    // Make commit. Rollback is conntrolled automatically
//    if(res.result() == true){
//        dbh.commit();
//    }
    // ~Transaction
    //------------------------------------------------------------------------------------------------------------------

    return res;
}

BoolResult_t SqliteStorage::doUpgrade(qint32 oldVersion)
{
    DbHelper dbh(name());
    if(!dbh.result().result()){
        return {false, dbh.result().data().toString()};
    }
    // If all ok at start point
    QSqlDatabase db = dbh.db();

    for(auto table: m_tableMap.values()){
        BoolVariantResult_t res = table->upgrade(db, oldVersion);
        if(!res.result()) {
            return {false, res.data().toString()};
        }
    }

    BoolVariantResult_t res = upgrade(db, oldVersion);
    if(!res.result()) {
        return {false, res.data().toString()};
    }

    return {true, ""};
}

void SqliteStorage::doFetchNotes(qint64 id)
{
    const qint32 max = 5;
    DbHelper dbh(name());
    if(!dbh.result().result()){
        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, max, max, dbh.result().data().toString()}, QVariant());
        return;
    }
    // If all ok at start point
    QSqlDatabase db = dbh.db();

    //
    NotesResult_t nr = _fetch(db);
    if(nr.result() != nullptr){
        emit fireTaskProgress({id, ProgressInfo::TPS_Success, 0, max, max, "ok"},
                              QVariant::fromValue(QSharedPointer<Notes>(nr.result())));
    }else{
        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, max, max, nr.data()}, QVariant());
    }
    //~

    return;
}

void SqliteStorage::doClearNotes(qint64 id)
{
    const qint32 max = 5;
    DbHelper dbh(name());
    if(!dbh.result().result()){
        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, max, max, dbh.result().data().toString()}, QVariant());
        return;
    }
    QSqlDatabase db = dbh.db();

    QSqlQuery q(db);
    q.prepare(QString("DELETE FROM %1").arg(TableNote::TBL_NAME));
    if(!q.exec()) {
        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, max, max, q.lastError().text()}, QVariant());
        return;
    }

    emit fireTaskProgress({id, ProgressInfo::TPS_Success, 0, max, max, "ok"}, QVariant());
}

//void SqliteStorage::doAddNotes(qint64 id, const QSharedPointer<QObject> &notes)
//{
//    QSharedPointer<NoteModel> spnl = qSharedPointerDynamicCast<NoteModel>(notes);
//    const qint32 cnt = spnl->items()->items.count();
//    if(cnt == 0) {
//        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, 0, 0, tr("Empty list of notes", "view->setCurrentIndex(fsModel->index(0, 0));DB")},
//                              QVariant());
//        return;
//    }

//    DbHelper dbh(name());
//    QSqlDatabase db = dbh.db();
//    if(!db.isOpen()){
//        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, 0, 0, dbh.lastError()}, QVariant());
//        return;
//    }

//    //------------------------------------------------------------------------------------------------------------------
//    // Transaction

//    const bool ok = db.transaction();
//    if(!ok) {
//        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, 0, 0, tr("transaction not started", "DB")}, QVariant());
//        return;
//    }


//    const QString QRY_INSERT(QStringLiteral("insert into %1(%2, %3, %4, %5, %6) values(?, ?, ?, ?, ?)")
//                             .arg(TableNote::TBL_NAME)
//                             .arg(TableNote::FLD_TEXT).arg(TableNote::FLD_FK_AUTHOR)
//                             .arg(TableNote::FLD_TS_CREATE).arg(TableNote::FLD_TS_EDIT)
//                             .arg(TableNote::FLD_DEL));
//    QSqlQuery q(db);
//    if (!q.prepare(QRY_INSERT)){
//        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, 0, 0, q.lastError().text()}, QVariant());
//    }else{
//        qint32 errors = 0;
//        qint64 ts = QDateTime::currentMSecsSinceEpoch();
//        for(int i=0; i<cnt; ++i) {
//            Note *note = spnl->items()->items.at(i);
//            q.addBindValue(note->text());
//            q.addBindValue(note->authorId());
//            q.addBindValue(ts);
//            q.addBindValue(ts);
//            q.addBindValue(0);
//            if (!q.exec()){
//                ++errors;
//            }else{
//                note->setId(q.lastInsertId().toInt());
//            }
//        }

//        if(!errors) {
//            emit fireTaskProgress({id, ProgressInfo::TPS_Success, 0, 0, 0, ""},
//                                  QVariant::fromValue(notes));
//        }else{
//            emit fireTaskProgress({id, ProgressInfo::TPS_Warning, 0, 0, 0, tr("%1 errors inserting notes").arg(errors)},
//                                  QVariant::fromValue(notes));
//        }
//    }

//    // Make commit or rollback
//    db.commit();

//    // ~Transaction
//    //------------------------------------------------------------------------------------------------------------------

//}

//void SqliteStorage::doAddNotes2(qint64 id, const QVector<QSharedPointer<Note> > &notes)
//{
////    QSharedPointer<NoteModel> spnl = qSharedPointerDynamicCast<NoteModel>(notes);
//    const qint32 cnt = notes.count();
//    if(cnt == 0) {
//        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, 0, 0, tr("Empty list of notes", "view->setCurrentIndex(fsModel->index(0, 0));DB")},
//                              QVariant());
//        return;
//    }

//    DbHelper dbh(name());
//    QSqlDatabase db = dbh.db();
//    if(!db.isOpen()){
//        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, 0, 0, dbh.lastError()}, QVariant());
//        return;
//    }

//    //------------------------------------------------------------------------------------------------------------------
//    // Transaction

//    const bool ok = db.transaction();
//    if(!ok) {
//        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, 0, 0, tr("transaction not started", "DB")}, QVariant());
//        return;
//    }


//    const QString QRY_INSERT(QStringLiteral("insert into %1(%2, %3, %4, %5, %6) values(?, ?, ?, ?, ?)")
//                             .arg(TableNote::TBL_NAME)
//                             .arg(TableNote::FLD_TEXT).arg(TableNote::FLD_FK_AUTHOR)
//                             .arg(TableNote::FLD_TS_CREATE).arg(TableNote::FLD_TS_EDIT)
//                             .arg(TableNote::FLD_DEL));
//    QSqlQuery q(db);
//    if (!q.prepare(QRY_INSERT)){
//        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, 0, 0, q.lastError().text()}, QVariant());
//    }else{
//        qint32 errors = 0;
//        qint64 ts = QDateTime::currentMSecsSinceEpoch();
//        for(int i=0; i<cnt; ++i) {
//            Note *note = notes.at(i).data();
//            q.addBindValue(note->text());
//            q.addBindValue(note->authorId());
//            q.addBindValue(ts);
//            q.addBindValue(ts);
//            q.addBindValue(0);
//            if (!q.exec()){
//                ++errors;
//            }else{
//                note->setId(q.lastInsertId().toInt());
//            }
//        }

//        if(!errors) {
//            emit fireTaskProgress({id, ProgressInfo::TPS_Success, 0, 0, 0, ""},
//                                  QVariant::fromValue(notes));
//        }else{
//            emit fireTaskProgress({id, ProgressInfo::TPS_Warning, 0, 0, 0, tr("%1 errors inserting notes").arg(errors)},
//                                  QVariant::fromValue(notes));
//        }
//    }

//    // Make commit or rollback
//    db.commit();

//    // ~Transaction
//    //------------------------------------------------------------------------------------------------------------------

//}

void SqliteStorage::doAddNotes3(qint64 id, const QSharedPointer<Notes> &notes)
{
    const qint32 cnt = notes->items.count();
    if(cnt == 0) {
        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, 0, 0, tr("Empty list of notes", "view->setCurrentIndex(fsModel->index(0, 0));DB")},
                              QVariant());
        return;
    }

    DbHelper dbh(name());
    if(!dbh.result().result()){
        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, 0, 0, dbh.result().data().toString()}, QVariant());
        return;
    }
    QSqlDatabase db = dbh.db();

//    //------------------------------------------------------------------------------------------------------------------
//    // Transaction

//    const bool ok = db.transaction();
//    if(!ok) {
//        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, 0, 0, tr("transaction not started", "DB")}, QVariant());
//        return;
//    }

    const QString QRY_INSERT(QStringLiteral("insert into %1(%2, %3, %4, %5, %6) values(?, ?, ?, ?, ?)")
                             .arg(TableNote::TBL_NAME)
                             .arg(TableNote::FLD_TEXT).arg(TableNote::FLD_FK_AUTHOR)
                             .arg(TableNote::FLD_TS_CREATE).arg(TableNote::FLD_TS_EDIT)
                             .arg(TableNote::FLD_DEL));
    QSqlQuery q(db);
    if (!q.prepare(QRY_INSERT)){
        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, 0, 0, q.lastError().text()}, QVariant());
    }else{
        qint32 errors = 0;
        qint64 ts = QDateTime::currentMSecsSinceEpoch();
        LOG_TP(ts);
        LOG_TP(QDateTime::fromMSecsSinceEpoch(ts).toString());
        for(int i=0; i<cnt; ++i) {
            Note *note = notes->items.at(i);
            q.addBindValue(note->text());
            q.addBindValue(note->authorId());
            q.addBindValue(ts);
            q.addBindValue(ts);
            q.addBindValue(0);
            if (!q.exec()){
                ++errors;
            }else{
                note->setId(q.lastInsertId().toInt());
                note->setTsCreated(ts);
                note->setTsEdited(ts);
            }
        }

        // Fetch after change
        NotesResult_t nr = _fetch(db);
        if(nr.result() != nullptr){
            QVariant v = QVariant::fromValue(QSharedPointer<Notes>(nr.result()));
            if(!errors) {
                emit fireTaskProgress({id, ProgressInfo::TPS_Success, 0, 0, 0, ""}, v);
            }else{
                emit fireTaskProgress({id, ProgressInfo::TPS_Warning, 0, 0, 0,
                                       tr("%1 errors inserting notes").arg(errors)}, v);
            }
        }else{
            emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, 0, 0, nr.data()}, QVariant());
        }
        //~


        // Commented 16/06/06
        //            if(!errors) {
        ////                emit fireTaskProgress({id, ProgressInfo::TPS_Success, 0, 0, 0, ""},
        ////                                      QVariant::fromValue(notes));
        //                emit fireTaskNoteProgress({id, ProgressInfo::TPS_Success, 0, 0, 0, ""}, notes);
        //            }else{
        ////                emit fireTaskProgress({id, ProgressInfo::TPS_Warning, 0, 0, 0, tr("%1 errors inserting notes").arg(errors)},
        ////                                      QVariant::fromValue(notes));

        //                int i = 10;
        //                emit fireTaskNoteProgress({id, ProgressInfo::TPS_Warning, 0, 0, 0, tr("%1 errors inserting notes").arg(errors)},
        //                                      notes);
        //            }
    }

    // ~Transaction
    //------------------------------------------------------------------------------------------------------------------
}

void SqliteStorage::doMarkNotesAsDeleted(qint64 id, const QVector<qint32> &ids)
{
    DbHelper dbh(name());
    if(!dbh.result().result()){
        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, 0, 0, dbh.result().data().toString()}, QVariant());
        return;
    }
    QSqlDatabase db = dbh.db();

    //------------------------------------------------------------------------------------------------------------------
    // Transaction
//    const bool ok = db.transaction();
//    if(!ok) {
//        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, 0, 0, tr("transaction not started", "DB")}, QVariant());
//        return;
//    }

    qint64 ts = QDateTime::currentMSecsSinceEpoch();
    const QString QRY_UPD(QStringLiteral("UPDATE %1 SET %2=%3, %4=%5 WHERE %6=?")
                          .arg(TableNote::TBL_NAME)
                          .arg(TableNote::FLD_DEL).arg(1)
                          .arg(TableNote::FLD_TS_EDIT).arg(ts)
                          .arg(TableNote::FLD_ID));
    QSqlQuery q(db);
    if (!q.prepare(QRY_UPD)){
        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, 0, 0, q.lastError().text()}, QVariant());
    }else{
        qint32 errors = 0;
        const int cnt = ids.count();
        for(int i=0; i<cnt; ++i) {
            qint32 id = ids.at(i);
            q.addBindValue(id);
            if (!q.exec()){
                ++errors;
            }
        }

        // Fetch after change
        NotesResult_t nr = _fetch(db);
        if(nr.result() != nullptr){
            QVariant v = QVariant::fromValue(QSharedPointer<Notes>(nr.result()));
            if(!errors) {
                emit fireTaskProgress({id, ProgressInfo::TPS_Success, 0, 0, 0, ""}, v);
            }else{
                emit fireTaskProgress({id, ProgressInfo::TPS_Warning, 0, 0, 0,
                                       tr("%1 errors inserting notes").arg(errors)}, v);
            }
        }else{
            emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, 0, 0, nr.data()}, QVariant());
        }
        //~

    }
}

void SqliteStorage::doRemoveNotes(qint64 id, const QVector<qint32> &ids)
{
}

void SqliteStorage::doFetchAuthors(qint64 id)
{
    const qint32 max = 5;

    DbHelper dbh(name());
    if(!dbh.result().result()){
        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, 0, 0, dbh.result().data().toString()}, QVariant());
        return;
    }
    QSqlDatabase db = dbh.db();


    QSqlQuery q(db);
    q.prepare(QString("SELECT * FROM %1").arg(TableAuthor::TBL_NAME));
    if(!q.exec()) {
        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, max, max, q.lastError().text()}, QVariant());
        return;
    }

    model::AuthorModel *items = new model::AuthorModel;
    while (q.next()) {
        Author *item = new Author;
        item->setId(q.value(TableAuthor::FLD_ID).toInt());
        item->setTitle(q.value(TableAuthor::FLD_TITLE).toString());
        items->data().append(item);
    }

    emit fireTaskProgress({id, ProgressInfo::TPS_Success, 0, max, max, "ok"}, QVariant::fromValue(QSharedPointer<QObject>(items)));
}

SqliteStorage::NotesResult_t SqliteStorage::_fetch(QSqlDatabase db)
{
    QSqlQuery q(db);
    // 16/06/06:
    //    QString fetchStr = QString("SELECT * FROM %1 WHERE %2 = 0").arg(TableNote::TBL_NAME).arg(TableNote::FLD_DEL);
    //    LOG_TP2(fetchStr << BaseStorage::NEW_VERSION << db.databaseName());

    q.prepare(QString("SELECT * FROM %1 WHERE %2 = 0").arg(TableNote::TBL_NAME).arg(TableNote::FLD_DEL));
    if(!q.exec()) {
        return {nullptr, q.lastError().text()};
    }

    Notes *notes = new Notes;
    while (q.next()) {
        qint32 fk_auth = q.value(TableNote::FLD_FK_AUTHOR).toInt();
        qint32 id = q.value(TableNote::FLD_ID).toInt();
        bool asDel = q.value(TableNote::FLD_DEL).toBool();
        Note *note = new Note();
        note->setId(id);
        note->setAuthorId(fk_auth);
        note->setText(q.value(TableNote::FLD_TEXT).toString());
        note->setAsDel(asDel);
        const qint64 ts = q.value(TableNote::FLD_TS_CREATE).toLongLong();
        note->setTsCreated(ts);
        note->setTsEdited(q.value(TableNote::FLD_TS_EDIT).toInt());
        notes->items.append(note);
    }
    return {notes, ""};
}

BoolResult_t SqliteStorage::upgradeFromV1()
{
    DbHelper dbh(name());
    if(!dbh.result().result()){
        return {false, dbh.result().data().toString()};
    }
    QSqlDatabase db = dbh.db();

    for(auto table: m_tableMap.values()){
        BoolVariantResult_t res = table->upgrade(db, 1);
        if(!res.result()) {
            return {false, res.data().toString()};
        }
    }

    // if ok
    {
        const QString strFindDefAuthorId =
                QString(QStringLiteral("SELECT %1 FROM %2 WHERE %3 = '%4'")
                        .arg(TableAuthor::FLD_ID).arg(TableAuthor::TBL_NAME)
                        .arg(TableAuthor::FLD_TITLE).arg(Constants::DEFAULT_AUTHOR));
        QSqlQuery q(db);
        if (!q.exec(strFindDefAuthorId)){
            return {false, q.lastError().text()};
        }else{
            if(q.next()){
                qint32 authId = q.value(0).toInt();
                const QString updateAuth =
                        QString(QStringLiteral("UPDATE %1 SET %2=%3 WHERE %4 is NULL"))
                        .arg(TableNote::TBL_NAME)
                        .arg(TableNote::FLD_FK_AUTHOR).arg(authId)
                        .arg(TableNote::FLD_FK_AUTHOR);
                if (!q.exec(updateAuth)){
                    return {false, q.lastError().text()};
                }
            }else{
                return {false, Q_FUNC_INFO};
            }
        }
    }

    {
        // Check if Note hasn't any tag - add default one.
        // SELECT tbl_notes.id FROM tbl_notes LEFT OUTER JOIN tbl_tags_notes ON tbl_notes.id = fk_notes WHERE fk_notes IS NULL
        const QString strFindOrphanNotes =
                QString(QStringLiteral("SELECT %1.%2 FROM %1 LEFT OUTER JOIN %3 ON %1.%2 = %4 WHERE %4 IS NULL")
                        .arg(TableNote::TBL_NAME).arg(TableNote::FLD_ID)
                        .arg(TableTagsNotes::TBL_NAME).arg(TableTagsNotes::FLD_FK_NOTES));

        QSqlQuery q(db);
        if (!q.exec(strFindOrphanNotes)){
            return {false, q.lastError().text()};
        }

        QSqlQuery qInsert(db);
        const QString QRY_INSERT(
                    QStringLiteral("insert into %1(%2, %3) values(?, ?)")
                    .arg(TableTagsNotes::TBL_NAME).arg(TableTagsNotes::FLD_FK_NOTES).arg(TableTagsNotes::FLD_FK_TAGS));
        if (!qInsert.prepare(QRY_INSERT)){
            return {false, qInsert.lastError().text()};
        }
        while (q.next()) {
            qInsert.addBindValue(q.value(TableNote::FLD_ID));
            qInsert.addBindValue(1);
            if (!qInsert.exec()){
                return {false, qInsert.lastError().text()};
            }
        }

    }

    // Make commit or rollbackTableAuthor
    return {true, ""};
}

vfx_shared::BoolResult_t SqliteStorage::upgradeFromV2()
{
    DbHelper dbh(name());
    if(!dbh.result().result()){
        return {false, dbh.result().data().toString()};
    }
    QSqlDatabase db = dbh.db();

    for(auto table: m_tableMap.values()){
        BoolVariantResult_t res = table->upgrade(db, 2);
        if(!res.result()) {
            return {false, res.data().toString()};
        }
    }

    // if ok
    BoolResult_t res(true, "");
    {
        // 'Config' table and data
//        BoolResult_t created = TableCfg().init(db, 2);
//        if (!created.result()){
//            return {created.result(), created.data()};
//        }
    }
//    qint32 defTagId = 0;
    {
//        // 'Tag' table and default data
//        BoolResult_t created = TableTags().init(db, TableTags::INVALID_VERSION);
//        if (!created.result()){
//            return {created.result(), created.data()};
//        }
//        defTagId = created.data().toInt();
    }
    {
//        // 'Note' table and data
//        BoolResult_t created = TableNote().init(db, 2);
//        if (!created.result()){
//            return {created.result(), created.data()};
//        }
    }
    {
//        BoolResult_t created = TableAuthor().init(db, 2);
//        if (!created.result()){
//            return {created.result(), created.data()};
//        }
    }
    {
//        // 'TagsNotes' table
//        BoolResult_t created = TableTagsNotes().init(db, TableTagsNotes::INVALID_VERSION);
//        if (!created.result()){
//            return {created.result(), created.data()};
//        }

        qint32 tagId = 0;
        const QString strIncomingTagId =
                QString(QStringLiteral("SELECT %1 FROM %2 WHERE %3 = '%4'")
                        .arg(TableTags::FLD_ID).arg(TableTags::TBL_NAME)
                        .arg(TableTags::FLD_TEXT).arg(Constants::TAG_INCOMING));
        QSqlQuery q(db);
        if (!q.exec(strIncomingTagId)){
            return {false, q.lastError().text()};
        }else{
            if(q.next()){
                tagId = q.value(0).toInt();
            }else{
                return {false, Q_FUNC_INFO};
            }
        }

        // Check if Note hasn't any tag - add default one.
        // SELECT tbl_notes.id FROM tbl_notes LEFT OUTER JOIN tbl_tags_notes ON tbl_notes.id = fk_notes WHERE fk_notes IS NULL
        const QString strFindOrphanNotes =
                QString(QStringLiteral("SELECT %1.%2 FROM %1 LEFT OUTER JOIN %3 ON %1.%2 = %4 WHERE %4 IS NULL")
                        .arg(TableNote::TBL_NAME).arg(TableNote::FLD_ID)
                        .arg(TableTagsNotes::TBL_NAME).arg(TableTagsNotes::FLD_FK_NOTES));

        if (!q.exec(strFindOrphanNotes)){
            return {false, q.lastError().text()};
        }

        QSqlQuery qInsert(db);
        const QString QRY_INSERT(
                    QStringLiteral("insert into %1(%2, %3) values(?, ?)")
                    .arg(TableTagsNotes::TBL_NAME).arg(TableTagsNotes::FLD_FK_NOTES).arg(TableTagsNotes::FLD_FK_TAGS));
        if (!qInsert.prepare(QRY_INSERT)){
            return {false, qInsert.lastError().text()};
        }
        while (q.next()) {
            qInsert.addBindValue(q.value(TableNote::FLD_ID));
            qInsert.addBindValue(tagId);
            if (!qInsert.exec()){
                return {false, qInsert.lastError().text()};
            }
        }

    }

    return res;
}

