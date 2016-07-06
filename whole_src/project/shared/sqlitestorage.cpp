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

// To simplify standard prefix in access-methods
#define PROVIDE_WRITABLE_ENGINE(engine_ptr) \
    EngineResult_t engRes = makeWritableEngine(); \
    SqlEngineSharedPtr_t engine_ptr = engRes.result(); \
    if(engine_ptr.isNull()) { \
        emit fireTaskProgress({taskId, ProgressInfo::TPS_Error, 0, 0, 0, engRes.data()}, QVariant()); \
        return; \
    }


SqliteStorage &SqliteStorage::instance()
{
    static SqliteStorage singleton;
    return singleton;
}

SqliteStorage::SqliteStorage(QObject *parent) : Inhherited_t(parent)
{
    INC_THIS(false);
    m_tableMap[TableCfg::TBL_NAME] = StoragePtr_t(new TableCfg);
    m_tableMap[TableNote::TBL_NAME] = StoragePtr_t(new TableNote);
    m_tableMap[TableAuthor::TBL_NAME] = StoragePtr_t(new TableAuthor);
    m_tableMap[TableTags::TBL_NAME] = StoragePtr_t(new TableTags);
    m_tableMap[TableTagsNotes::TBL_NAME] = StoragePtr_t(new TableTagsNotes);
    m_tableMap[TableTmpFromSrv::TBL_NAME] = StoragePtr_t(new TableTmpFromSrv);
}

SqliteStorage::~SqliteStorage()
{
    DEC_THIS(false);
}

BoolVariantResult_t SqliteStorage::tables() const
{
    EngineResult_t engRes = makeWritableEngine();
    SqlEngineSharedPtr_t sp = engRes.result();
    if(sp.isNull()) {
        return {false, engRes.data()};
    }
    return {true, sp->db().tables()};
}

BoolVariantResult_t SqliteStorage::doCreate(const SqlEngine_t &engine)
{
    // If all ok at start point
    QSqlDatabase db = engine->db();

    bool f = db.driver()->hasFeature(QSqlDriver::LastInsertId);
    if(!f) {
        return {false, "No feature 'QSqlDriver::LastInsertId'"};
    }

    for(StoragePtr_t table: m_tableMap.values()){
        BoolVariantResult_t res = table->create(engine);
        if(!res.result()) {
            return {false, res.data().toString()};
        }
    }
    return {true, QVariant()};
}

BoolVariantResult_t SqliteStorage::doUpgrade(const SqlEngine_t &engine, const V1_t &dummy)
{

    for(StoragePtr_t table: m_tableMap.values()){
        // dummy.value automatically dispathes
        BoolVariantResult_t res = table->upgrade(engine, dummy.value);
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
        QSqlQuery q(engine->db());
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

        QSqlQuery q(engine->db());
        if (!q.exec(strFindOrphanNotes)){
            return {false, q.lastError().text()};
        }

        QSqlQuery qInsert(engine->db());
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

BoolVariantResult_t SqliteStorage::doUpgrade(const SqlEngine_t &engine, const V2_t &dummy)
{

    for(StoragePtr_t table: m_tableMap.values()){
        // dummy.value automatically dispathes
        BoolVariantResult_t res = table->upgrade(engine, dummy.value);
        if(!res.result()) {
            return {false, res.data().toString()};
        }
    }

    qint32 tagId = 0;
    const QString strIncomingTagId =
            QString(QStringLiteral("SELECT %1 FROM %2 WHERE %3 = '%4'")
                    .arg(TableTags::FLD_ID).arg(TableTags::TBL_NAME)
                    .arg(TableTags::FLD_TEXT).arg(Constants::TAG_INCOMING));
    QSqlQuery q(engine->db());
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

    QSqlQuery qInsert(engine->db());
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

BoolVariantResult_t SqliteStorage::doUpgrade(const SqlEngine_t &engine, const V3_t &dummy)
{
    for(StoragePtr_t table: m_tableMap.values()){
        // dummy.value automatically dispathes
        BoolVariantResult_t res = table->upgrade(engine, dummy.value);
        if(!res.result()) {
            return {false, res.data().toString()};
        }
    }

    qint32 tagId = 0;
    const QString strIncomingTagId =
            QString(QStringLiteral("SELECT %1 FROM %2 WHERE %3 = '%4'")
                    .arg(TableTags::FLD_ID).arg(TableTags::TBL_NAME)
                    .arg(TableTags::FLD_TEXT).arg(Constants::TAG_INCOMING));
    QSqlQuery q(engine->db());
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
    const QString strFindOrphanNotes =
            QString(QStringLiteral("SELECT %1.%2 FROM %1 LEFT OUTER JOIN %3 ON %1.%2 = %4 WHERE %4 IS NULL")
                    .arg(TableNote::TBL_NAME).arg(TableNote::FLD_ID)
                    .arg(TableTagsNotes::TBL_NAME).arg(TableTagsNotes::FLD_FK_NOTES));

    if (!q.exec(strFindOrphanNotes)){
        return {false, q.lastError().text()};
    }

    QSqlQuery qInsert(engine->db());
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

BoolVariantResult_t SqliteStorage::doUpgrade(const SqliteStorage::SqlEngine_t &engine, const V4_t &dummy)
{
    for(StoragePtr_t table: m_tableMap.values()){
        // dummy.value automatically dispathes
        BoolVariantResult_t res = table->upgrade(engine, dummy.value);
        if(!res.result()) {
            return {false, res.data().toString()};
        }
    }

    return {true, dummy.value};
}

void SqliteStorage::doReset(SqlEngineSharedPtr_t engine)
{
    Q_UNUSED(engine);
}

BaseStorage<SqlEngineSharedPtr_t>::EngineResult_t SqliteStorage::makeWritableEngine() const
{
    SqlEngineSharedPtr_t sp(SqlEngine::makeWritable(name()));
    if(!sp->innerState().result()){
        return {SqlEngineSharedPtr_t(nullptr), sp->innerState().data().toString()};
    }
    return {sp, QString()};
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

void SqliteStorage::doCreateItems(SqlEngineSharedPtr_t engine)
{
    Q_UNUSED(engine);
    //    BaseTable *table = new TableNote(this);
    //    m_items.insert(table->name(), table);
    //    table = new TableCfg(this);
    //    m_items.insert(table->name(), table);
}

BoolVariantResult_t SqliteStorage::doCheckVersion(SqlEngineSharedPtr_t engine) const
{
//    EngineResult_t engRes = getEngine();
//    SqlEngineSharedPtr_t sp = engRes.result();
//    if(sp.isNull()) {
//        return {false, engRes.data()};
//    }
    // If all ok at start point
    QSqlDatabase db = engine->db();

    // Check row count
    {
        QSqlQuery q(db);
        q.prepare(QString("SELECT COUNT (*) FROM tbl_config WHERE the_key = 'version'"));
        bool isExec = q.exec();
        if(!isExec) {
            QString str = q.lastError().text();
            return {false, str};
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

//vfx_shared::BoolResult_t SqliteStorage::doCreate()
//{
////    //------------------------------------------------------------------------------------------------------------------
////    // Transaction opens in DbHelper
////    EngineResult_t engRes = getEngine();
////    SqlEngineSharedPtr_t sp = engRes.result();
////    if(sp.isNull()) {
////        return {false, engRes.data()};
////    }
////    // If all ok at start point
////    QSqlDatabase db = sp->db();

////    BoolResult_t res(true, "");
////    bool f = db.driver()->hasFeature(QSqlDriver::LastInsertId);
////    if(!f) {
////        return {false, "No feature 'QSqlDriver::LastInsertId'"};
////    }

////    for(StoragePtr_t table: m_tableMap.values()){
////        BoolVariantResult_t res = table->create(sp);
////        if(!res.result()) {
////            return {false, res.data().toString()};
////        }
////    }

////    // if ok
////    {
////        // 'Config' table and data
//////        BoolResult_t created = TableCfg().init(db, TableNote::INVALID_VERSION);
//////        if (!created.result()){
//////            return {created.result(), created.data()};
//////        }
////    }

////    {
////        // 'Author' table and data
//////        BoolResult_t created = TableAuthor().init(db, TableAuthor::INVALID_VERSION);
//////        if (!created.result()){
//////            return {created.result(), created.data()};
//////        }
////    }

////    {
//////        // Table Note
//////        BoolResult_t created = TableNote().init(db, TableNote::INVALID_VERSION);
//////        if (!created.result()){
//////            return {created.result(), created.data()};
//////        }
////    }

////    {
////        // 'Tag' table and default data
//////        BoolResult_t created = TableTags().init(db, TableTags::INVALID_VERSION);
//////        if (!created.result()){
//////            return {created.result(), created.data()};
//////        }
////    }

////    {
////        // 'TagsNotes' table
//////        BoolResult_t created = TableTagsNotes().init(db, TableTagsNotes::INVALID_VERSION);
//////        if (!created.result()){
//////            return {created.result(), created.data()};
//////        }
////    }

////    // Make commit. Rollback is conntrolled automatically
//////    if(res.result() == true){
//////        dbh.commit();
//////    }
////    // ~Transaction
////    //------------------------------------------------------------------------------------------------------------------

////    return res;
//}

//BoolResult_t SqliteStorage::doUpgrade(qint32 oldVersion)
//{
//    EngineResult_t engRes = makeWritableEngine();
//    SqlEngineSharedPtr_t sp = engRes.result();
//    if(sp.isNull()) {
//        return {false, engRes.data()};
//    }
//    // If all ok at start point
//    QSqlDatabase db = sp->db();

//    for(StoragePtr_t table: m_tableMap.values()){
//        BoolVariantResult_t res = table->upgrade(sp, oldVersion);
//        if(!res.result()) {
//            return {false, res.data().toString()};
//        }
//    }

//    BoolVariantResult_t res = upgrade(sp, oldVersion);
//    if(!res.result()) {
//        return {false, res.data().toString()};
//    }

//    return {true, ""};
//}

void SqliteStorage::doFetchNotes(qint64 taskId, const UuidVector_t &ids)
{
    NotesResult_t nr(nullptr, QString());
    {
        // Inner namespace for control DB lifetime
        PROVIDE_WRITABLE_ENGINE(enginePtr);
        // If all ok at start point
        QSqlDatabase db = enginePtr->db();

        nr = _fetch(db, ids);
    }
    if(nr.result() != nullptr){
        emit fireTaskProgress({taskId, ProgressInfo::TPS_Success, 0, 0, 0, "ok"},
                              QVariant::fromValue(QSharedPointer<Notes>(nr.result())));
    }else{
        emit fireTaskProgress({taskId, ProgressInfo::TPS_Error, 0, 0, 0, nr.data()}, QVariant());
    }
    return;
}

void SqliteStorage::doClearNotes(qint64 taskId)
{
    const qint32 max = 5;
    EngineResult_t engRes = makeWritableEngine();
    SqlEngineSharedPtr_t sp = engRes.result();
    if(sp.isNull()) {
        emit fireTaskProgress({taskId, ProgressInfo::TPS_Error, 0, max, max, engRes.data()}, QVariant());
        return;
    }
    // If all ok at start point
    QSqlDatabase db = sp->db();

    QSqlQuery q(db);
    q.prepare(QString("DELETE FROM %1").arg(TableNote::TBL_NAME));
    if(!q.exec()) {
        emit fireTaskProgress({taskId, ProgressInfo::TPS_Error, 0, max, max, q.lastError().text()}, QVariant());
        return;
    }

    emit fireTaskProgress({taskId, ProgressInfo::TPS_Success, 0, max, max, "ok"}, QVariant());
}

//void SqliteStorage::doAddNote(qint64 id, const QSharedPointer<Note> &note)
//{
//    PROVIDE_WRITABLE_ENGINE(engine);
//    // If all ok at start point
//    QSqlDatabase db = engine->db();

//    const QString QRY_INSERT(QStringLiteral("insert into %1(%2, %3, %4, %5, %6, %7) "
//                                            "values(:uuid, :text, :author, :ts_create, :ts_edit, :del)")
//                             .arg(TableNote::TBL_NAME)
//                             .arg(TableNote::FLD_UUID)
//                             .arg(TableNote::FLD_TEXT).arg(TableNote::FLD_FK_AUTHOR)
//                             .arg(TableNote::FLD_TS_CREATE).arg(TableNote::FLD_TS_EDIT)
//                             .arg(TableNote::FLD_DEL));
//    QSqlQuery q(db);
//    if (!q.prepare(QRY_INSERT)){
//        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, 0, 0, q.lastError().text()}, QVariant());
//    }else{
//        q.bindValue(":uuid", note->uuid());
//        q.bindValue(":text", note->text());
//        q.bindValue(":author", note->authorId());
//        q.bindValue(":ts_create", note->tsCreated());
//        q.bindValue(":ts_edit", note->tsEdited());
//        q.bindValue(":del", 0);
//        if (!q.exec()){
//            emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, 0, 0, q.lastError().text()}, QVariant());
//        }
//    }
//    QVariant v = QVariant::fromValue(note);
//    emit fireTaskProgress({id, ProgressInfo::TPS_Success, 0, 0, 0, ""}, v);
//}

void SqliteStorage::doAddNotes(qint64 taskId, const QSharedPointer<Notes> &notes)
{
    const qint32 cnt = notes->items.count();
    if(cnt == 0) {
        emit fireTaskProgress({taskId, ProgressInfo::TPS_Error, 0, 0, 0, tr("Empty list of notes", "view->setCurrentIndex(fsModel->index(0, 0));DB")},
                              QVariant());
        return;
    }

    qint32 errors = 0;
    {
        // Inner anonimous namespace for shortage DB-access lifecycle
        PROVIDE_WRITABLE_ENGINE(engine);
        // If all ok at start point
        QSqlDatabase db = engine->db();
        const QString QRY_INSERT(QStringLiteral("insert into %1(%2, %3, %4, %5, %6, %7) values(?, ?, ?, ?, ?, ?)")
                                 .arg(TableNote::TBL_NAME)
                                 .arg(TableNote::FLD_UUID).arg(TableNote::FLD_TEXT).arg(TableNote::FLD_FK_AUTHOR)
                                 .arg(TableNote::FLD_TS_CREATE).arg(TableNote::FLD_TS_EDIT).arg(TableNote::FLD_DEL));
        QSqlQuery q(db);
        if (!q.prepare(QRY_INSERT)){
            emit fireTaskProgress({taskId, ProgressInfo::TPS_Error, 0, 0, 0, q.lastError().text()}, QVariant());
        }else{
            for(int i=0; i<cnt; ++i) {
                Note *note = notes->items.at(i);
                q.addBindValue(note->uuid());
                q.addBindValue(note->text());
                q.addBindValue(note->authorId());
                q.addBindValue(note->tsCreated());
                q.addBindValue(note->tsEdited());
                q.addBindValue(0);
                if (!q.exec()){
                    ++errors;
                }
            }
        }
    }
    QVariant v = QVariant::fromValue(QSharedPointer<Notes>(notes));
    if(!errors) {
        emit fireTaskProgress({taskId, ProgressInfo::TPS_Success, 0, 0, 0, ""}, v);
    }else{
        emit fireTaskProgress({taskId, ProgressInfo::TPS_Warning, 0, 0, 0,
                               tr("%1 errors inserting notes").arg(errors)}, v);
    }
}

void SqliteStorage::doEditNotes(qint64 taskId, const QSharedPointer<Notes> &notes)
{
    const qint32 cnt = notes->items.count();
    if(cnt == 0) {
        emit fireTaskProgress({taskId, ProgressInfo::TPS_Error, 0, 0, 0, tr("Empty list of notes", "view->setCurrentIndex(fsModel->index(0, 0));DB")},
                              QVariant());
        return;
    }

    qint32 errors = 0;
    {
        // Inner anonimous namespace for shortage DB-access lifecycle
        PROVIDE_WRITABLE_ENGINE(engine);
        // If all ok at start point
        QSqlDatabase db = engine->db();
        const QString QRY_UPD(QStringLiteral("UPDATE %1 SET %2=:text, %3=:author, %4=:ts_create, %5=:ts_edit, "
                                             "%6=:del WHERE %7=:uuid")
                              .arg(TableNote::TBL_NAME)
                              .arg(TableNote::FLD_TEXT).arg(TableNote::FLD_FK_AUTHOR)
                              .arg(TableNote::FLD_TS_CREATE).arg(TableNote::FLD_TS_EDIT)
                              .arg(TableNote::FLD_DEL).arg(TableNote::FLD_UUID));

        QSqlQuery q(db);
        if (!q.prepare(QRY_UPD)){
            emit fireTaskProgress({taskId, ProgressInfo::TPS_Error, 0, 0, 0, q.lastError().text()}, QVariant());
        }else{
            for(int i=0; i<cnt; ++i) {
                Note *note = notes->items.at(i);
                q.bindValue(":uuid", note->uuid());
                q.bindValue(":text", note->text());
                q.bindValue(":author", note->authorId());
                q.bindValue(":ts_create", note->tsCreated());
                q.bindValue(":ts_edit", note->tsEdited());
                q.bindValue(":del", note->asDel());
                if (!q.exec()){
                    ++errors;
                }
            }
        }
    }
    QVariant v = QVariant::fromValue(QSharedPointer<Notes>(notes));
    if(!errors) {
        emit fireTaskProgress({taskId, ProgressInfo::TPS_Success, 0, 0, 0, ""}, v);
    }else{
        emit fireTaskProgress({taskId, ProgressInfo::TPS_Warning, 0, 0, 0,
                               tr("%1 errors inserting notes").arg(errors)}, v);
    }
}

void SqliteStorage::doMarkNotesAsDeleted(qint64 taskId, const UuidVector_t &ids, bool setMark)
{
//    EngineResult_t engRes = makeWritableEngine();
//    SqlEngineSharedPtr_t sp = engRes.result();
//    if(sp.isNull()) {
//        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, 0, 0, engRes.data()}, QVariant());
//        return;
//    }
//    // If all ok at start point
//    QSqlDatabase db = sp->db();

    qint32 errors = 0;
    {
        // Inner anonimous namespace for shortage DB-access lifecycle
        PROVIDE_WRITABLE_ENGINE(engine);
        // If all ok at start point
        QSqlDatabase db = engine->db();
        qint64 ts = QDateTime::currentMSecsSinceEpoch();
        const QString QRY_UPD(QStringLiteral("UPDATE %1 SET %2=%3, %4=%5 WHERE %6=?")
                              .arg(TableNote::TBL_NAME)
                              .arg(TableNote::FLD_DEL).arg(setMark ? 1 : 0)
                              .arg(TableNote::FLD_TS_EDIT).arg(ts)
                              .arg(TableNote::FLD_UUID));
        QSqlQuery q(db);
        if (!q.prepare(QRY_UPD)){
            emit fireTaskProgress({taskId, ProgressInfo::TPS_Error, 0, 0, 0, q.lastError().text()}, QVariant());
        }else{
            qint32 errors = 0;
            UuidVector_t markedIds;
            const int cnt = ids.count();
            for(int i=0; i<cnt; ++i) {
                QString uuid = ids.at(i);
                q.addBindValue(uuid);
                if (!q.exec()){
                    ++errors;
                }else{
                    markedIds.append(uuid);
                }
            }
        }
    }
    QVariant v = QVariant::fromValue(ids);
    if(!errors) {
        emit fireTaskProgress({taskId, ProgressInfo::TPS_Success, 0, 0, 0, ""}, v);
    }else{
        emit fireTaskProgress({taskId, ProgressInfo::TPS_Warning, 0, 0, 0,
                               tr("%1 errors inserting notes").arg(errors)}, v);
    }

}

void SqliteStorage::doRemoveNotes(qint64 taskId, const UuidVector_t &ids)
{
    qint32 errors = 0;
    {
        // Inner namespace for control DB lifetime
        PROVIDE_WRITABLE_ENGINE(enginePtr);
        // If all ok at start point
        QSqlDatabase db = enginePtr->db();

        const QString QRY_DEL(QStringLiteral("DELETE FROM %1 WHERE %2=:uuid")
                              .arg(TableNote::TBL_NAME).arg(TableNote::FLD_UUID));
        QSqlQuery q(db);
        if (!q.prepare(QRY_DEL)){
            emit fireTaskProgress({taskId, ProgressInfo::TPS_Error, 0, 0, 0, q.lastError().text()}, QVariant());
        }else{
            UuidVector_t deletedIds;
            const int cnt = ids.count();
            for(int i=0; i<cnt; ++i) {
                UuidType_t id = ids.at(i);
    //            LOG_TP(id);
                q.bindValue(":uuid", id);
                if (!q.exec()){
                    ++errors;
                }else{
                    deletedIds.append(id);
                }
            }
        }
    }

    QVariant v = QVariant::fromValue(ids);
    if(!errors) {
        emit fireTaskProgress({taskId, ProgressInfo::TPS_Success, 0, 0, 0, ""}, v);
    }else{
        emit fireTaskProgress({taskId, ProgressInfo::TPS_Warning, 0, 0, 0,
                               tr("%1 errors inserting notes").arg(errors)}, v);
    }

}

void SqliteStorage::doFetchAuthors(qint64 taskId)
{
    model::AuthorModel *items = new model::AuthorModel;
    {
        // Inner namespace for control DB lifetime
        PROVIDE_WRITABLE_ENGINE(enginePtr);
        // If all ok at start point
        QSqlDatabase db = enginePtr->db();

        QSqlQuery q(db);
        q.prepare(QString("SELECT * FROM %1").arg(TableAuthor::TBL_NAME));
        if(!q.exec()) {
            emit fireTaskProgress({taskId, ProgressInfo::TPS_Error, 0, 0, 0, q.lastError().text()}, QVariant());
            return;
        }
        while (q.next()) {
            Author *item = new Author;
            item->setId(q.value(TableAuthor::FLD_ID).toInt());
            item->setTitle(q.value(TableAuthor::FLD_TITLE).toString());
            items->data().append(item);
        }
    }
    emit fireTaskProgress({taskId, ProgressInfo::TPS_Success, 0, 0, 0, "ok"},
                          QVariant::fromValue(QSharedPointer<QObject>(items)));
}

SqliteStorage::NotesResult_t SqliteStorage::_fetch(QSqlDatabase db, const UuidVector_t &ids)
{
    QSqlQuery q(db);
    if(!ids.count()) {
        q.prepare(QString("SELECT * FROM %1 WHERE %2 = 0 ORDER BY %3 DESC")
                  .arg(TableNote::TBL_NAME).arg(TableNote::FLD_DEL).arg(TableNote::FLD_TS_CREATE));
    }else{
        QString in;
        foreach (auto uuid, ids) {
            in.append('"').append(uuid).append('"').append(',');
        }
        in.chop(1);
        q.prepare(QString("SELECT * FROM %1 WHERE %2 IN (%3) ORDER BY %4 DESC")
                  .arg(TableNote::TBL_NAME).arg(TableNote::FLD_UUID).arg(in).arg(TableNote::FLD_TS_CREATE));
    }
    if(!q.exec()) {
        return {nullptr, q.lastError().text()};
    }

    Notes *notes = new Notes;
    while (q.next()) {
        qint32 fk_auth = q.value(TableNote::FLD_FK_AUTHOR).toInt();
        const QString uuid = q.value(TableNote::FLD_UUID).toString();

        bool asDel = q.value(TableNote::FLD_DEL).toBool();
        Note *note = new Note(uuid);
        note->setAuthorId(fk_auth);
        note->setText(q.value(TableNote::FLD_TEXT).toString());
        note->setAsDel(asDel);
        note->setTsCreated(q.value(TableNote::FLD_TS_CREATE).toLongLong());
        note->setTsEdited(q.value(TableNote::FLD_TS_EDIT).toLongLong());
        notes->items.append(note);
    }
    return {notes, ""};
}


