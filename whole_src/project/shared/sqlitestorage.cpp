#include <QSqlQuery>
#include <QSqlDriver>

#include "sqlitestorage.h"
#include "storage_globals.h"
#include "note.h"

// Include all old versions to make upgrade
#include "sqlite_storage_v1.h"
#include "sqlite_storage_v2.h"
// Include fresh version
#include "sqlite_storage_v3.h"

// make direct access to current version namespace.
using namespace storage::sqlite::v3;

using namespace storage;

SqliteStorage::SqliteStorage(QObject *parent) : Inhherited_t(parent)
{

}

SqliteStorage::~SqliteStorage()
{

}

BoolVariantResult_t SqliteStorage::tables() const
{
    DbHelper dbh(name());
    QSqlDatabase db = dbh.db();
    if(!db.isOpen()){
        return {false, dbh.lastError()};
    }

    return {true, db.tables()};
}

AuthorListResult_t SqliteStorage::getAuthors() const
{
    DbHelper dbh(name());
    QSqlDatabase db = dbh.db();
    if(!db.isOpen()){
        return {nullptr, dbh.lastError()};
    }

    QSqlQuery q(db);
    q.prepare(QString("SELECT * FROM %1").arg(TableAuthor::TBL_NAME));
    if(!q.exec()) {
        return {nullptr, q.lastError().text()};
    }

    model::AuthorList *items = new model::AuthorList;
    while (q.next()) {
        Author *item = new Author;
        item->setId(q.value(TableAuthor::FLD_ID).toInt());
        item->setTitle(q.value(TableAuthor::FLD_TITLE).toString());
        items->data().append(item);
    }
    return {items, ""};
}

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
    QSqlDatabase db = dbh.db();
    if(!db.isOpen()){
        return {false, dbh.lastError()};
    }

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

//    TableCfg *table = qobject_cast<TableCfg *>(m_items.value(TABLE_CONFIG_NAME));
//    if(table == nullptr){
//        return STORAGE_NO_EXISTS;
//    }

//    bool exists = table->contains("the_key", "version");
//    if(!exists) {
//        return STORAGE_NO_EXISTS;
//    }

//    qint32 version = table->get("the_key").toInt;
}

vfx_shared::BoolResult_t SqliteStorage::doCreate()
{
    DbHelper dbh(name());
    QSqlDatabase db = dbh.db();
    if(!db.isOpen()){
        return {false, dbh.lastError()};
    }

    //------------------------------------------------------------------------------------------------------------------
    // Transaction

    const bool ok = db.transaction();
    if(!ok) {
        return {false, tr("transaction not started", "DB")};
    }

    BoolResult_t res(true, "");
    bool f = db.driver()->hasFeature(QSqlDriver::LastInsertId);
    if(!f) {
        return {false, "No feature 'QSqlDriver::LastInsertId'"};
    }

    // if ok
    {
        // 'Config' table and data
        QSqlQuery q(db);
        if (!q.exec(TableCfg::QRY_CREATE)){
            return {false, q.lastError().text()};
        }
        const QString QRY_INSERT(QStringLiteral("insert into %1(%2, %3) values(?, ?)")
                                 .arg(TableCfg::TBL_NAME).arg(TableCfg::FLD_KEY).arg(TableCfg::FLD_VALUE));
        if (!q.prepare(QRY_INSERT)){
            return {false, q.lastError().text()};
        }
        q.addBindValue("version");
        q.addBindValue(BaseStorage::NEW_VERSION);
        if (!q.exec()){
            return {false, q.lastError().text()};
        }
    }

    {
        // 'Author' table and data
        BoolResult_t created = TableAuthor::create(db);
        if (!created.result()){
            return {created.result(), created.data()};
        }
    }

    {
        // 'Note' table and data
        QSqlQuery q(db);
        if (!q.exec(TableNote::QRY_CREATE)){
            return {false, q.lastError().text()};
        }
    }

    {
        // 'Tag' table and default data
        BoolResult_t created = TableTags::create(db);
        if (!created.result()){
            return {created.result(), created.data()};
        }
    }

    {
        // 'TagsNotes' table
        BoolResult_t created = TableTagsNotes::create(db);
        if (!created.result()){
            return {created.result(), created.data()};
        }
    }

    // Make commit or rollback
    if(res.result() == true){
        db.commit();
    }else{
        db.rollback();
    }

    // ~Transaction
    //------------------------------------------------------------------------------------------------------------------

    return res;
}

BoolResult_t SqliteStorage::doUpgrade(qint32 oldVersion)
{
    BoolResult_t res = RES_FALSE_EXT(tr("Upgrade failed", "DB"));

    switch (oldVersion) {
    case STORAGE_V1:
        res = upgradeFromV1();
        break;
    case STORAGE_V2:
        res = upgradeFromV2();
        break;
    default:
        break;
    }

    return res;
}

void SqliteStorage::doFetchNotes(qint64 id)
{
    const qint32 max = 5;
    DbHelper dbh(name());
    QSqlDatabase db = dbh.db();
    if(!db.isOpen()){
        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, max, max, dbh.lastError()}, QVariant());
        return;
//        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, max, max, dbh.lastError()}, QSharedPointer<QObject>());
    }

    QSqlQuery q(db);
    q.prepare(QString("SELECT * FROM %1").arg(TableNote::TBL_NAME));
    if(!q.exec()) {
        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, max, max, q.lastError().text()}, QVariant());
        return;
//        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, max, max, q.lastError().text()}, QSharedPointer<QObject>());
    }

    model::NoteList *notes = new model::NoteList;
    while (q.next()) {
        qint32 fk_auth = q.value(TableNote::FLD_FK_AUTHOR).toInt();
        qint32 id = q.value(TableNote::FLD_ID).toInt();
        Note *note = new Note();
        note->setId(id);
        note->setAuthorId(fk_auth);
        note->setText(q.value(TableNote::FLD_TEXT).toString());
        notes->data().append(note);
    }
    emit fireTaskProgress({id, ProgressInfo::TPS_Success, 0, max, max, "ok"}, QVariant::fromValue(QSharedPointer<QObject>(notes)));
//    emit fireTaskProgress({id, ProgressInfo::TPS_Success, 0, max, max, "ok"}, QSharedPointer<QObject>(notes));
    return;
}

void SqliteStorage::doClearNotes(qint64 id)
{
    const qint32 max = 5;
    DbHelper dbh(name());
    QSqlDatabase db = dbh.db();
    if(!db.isOpen()){
        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, max, max, dbh.lastError()}, QVariant());
        return;
    }

    QSqlQuery q(db);
    q.prepare(QString("DELETE FROM %1").arg(TableNote::TBL_NAME));
    if(!q.exec()) {
        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, max, max, q.lastError().text()}, QVariant());
        return;
    }

    emit fireTaskProgress({id, ProgressInfo::TPS_Success, 0, max, max, "ok"}, QVariant());
}

void SqliteStorage::doAddNotes(qint64 id, const QSharedPointer<QObject> &notes)
{
    QSharedPointer<NoteList> spnl = qSharedPointerDynamicCast<NoteList>(notes);
    const qint32 cnt = spnl->data().count();
    if(cnt == 0) {
        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, 0, 0, tr("Empty list of notes", "DB")},
                              QVariant());
        return;
    }

    DbHelper dbh(name());
    QSqlDatabase db = dbh.db();
    if(!db.isOpen()){
        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, 0, 0, dbh.lastError()}, QVariant());
        return;
    }

    //------------------------------------------------------------------------------------------------------------------
    // Transaction

    const bool ok = db.transaction();
    if(!ok) {
        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, 0, 0, tr("transaction not started", "DB")}, QVariant());
        return;
    }


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
        for(int i=0; i<cnt; ++i) {
            Note *note = spnl->data().at(i);
            q.addBindValue(note->text());
            q.addBindValue(note->authorId());
            q.addBindValue(ts);
            q.addBindValue(ts);
            q.addBindValue(0);
            if (!q.exec()){
                ++errors;
            }else{
                note->setId(q.lastInsertId().toInt());
            }
        }

        if(!errors) {
            emit fireTaskProgress({id, ProgressInfo::TPS_Success, 0, 0, 0, ""},
                                  QVariant::fromValue(notes));
        }else{
            emit fireTaskProgress({id, ProgressInfo::TPS_Warning, 0, 0, 0, tr("%1 errors inserting notes").arg(errors)},
                                  QVariant::fromValue(notes));
        }
    }

    // Make commit or rollback
    db.commit();

    // ~Transaction
    //------------------------------------------------------------------------------------------------------------------

}

void SqliteStorage::doFetchAuthors(qint64 id)
{
    const qint32 max = 5;

    DbHelper dbh(name());
    QSqlDatabase db = dbh.db();
    if(!db.isOpen()){
        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, max, max, dbh.lastError()}, QVariant());
        return;
    }

    QSqlQuery q(db);
    q.prepare(QString("SELECT * FROM %1").arg(TableAuthor::TBL_NAME));
    if(!q.exec()) {
        emit fireTaskProgress({id, ProgressInfo::TPS_Error, 0, max, max, q.lastError().text()}, QVariant());
        return;
    }

    model::AuthorList *items = new model::AuthorList;
    while (q.next()) {
        Author *item = new Author;
        item->setId(q.value(TableAuthor::FLD_ID).toInt());
        item->setTitle(q.value(TableAuthor::FLD_TITLE).toString());
        items->data().append(item);
    }

    emit fireTaskProgress({id, ProgressInfo::TPS_Success, 0, max, max, "ok"}, QVariant::fromValue(QSharedPointer<QObject>(items)));
}

BoolResult_t SqliteStorage::upgradeFromV1()
{
    DbHelper dbh(name());
    QSqlDatabase db = dbh.db();
    if(!db.isOpen()){
        return {false, dbh.lastError()};
    }

    //------------------------------------------------------------------------------------------------------------------
    // Transaction

    const bool ok = db.transaction();
    if(!ok) {
        return {false, tr("transaction not started", "DB")};
    }

    // if ok
    BoolResult_t res(true, "");
    qint64 ts = QDateTime::currentMSecsSinceEpoch();
    {
        // 'Config' table and data
        QSqlQuery q(db);
        const QString addDescr =
                QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 varchar"))
                .arg(TableCfg::TBL_NAME).arg(TableCfg::FLD_DESCR);
        if (!q.exec(addDescr)){
            return {false, q.lastError().text()};
        }
        const QString addTsCreate =
                QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer"))
                .arg(TableCfg::TBL_NAME).arg(TableCfg::FLD_TS_CREATE);
        if (!q.exec(addTsCreate)){
            return {false, q.lastError().text()};
        }
        const QString addTsEdit =
                QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer"))
                .arg(TableCfg::TBL_NAME).arg(TableCfg::FLD_TS_EDIT);
        if (!q.exec(addTsEdit)){
            return {false, q.lastError().text()};
        }

        const QString updateVer =
                QString(QStringLiteral("UPDATE %1 SET %2=%3, %4=%5, %6=%7 WHERE %8='%9'"))
                .arg(TableCfg::TBL_NAME)
                .arg(TableCfg::FLD_VALUE).arg(VERSION)
                .arg(TableCfg::FLD_TS_CREATE).arg(ts)
                .arg(TableCfg::FLD_TS_EDIT).arg(ts)
                .arg(TableCfg::FLD_KEY).arg("version");
        if (!q.exec(updateVer)){
            return {false, q.lastError().text()};
        }
    }
    qint32 systemAuthorId = 0;
    {
        // 'Author' table and data
        BoolResult_t created = TableAuthor::create(db);
        if (!created.result()){
            return {created.result(), created.data()};
        }
        systemAuthorId = created.data().toInt();
    }
    {
        // 'Tag' table and default data
        BoolResult_t created = TableTags::create(db);
        if (!created.result()){
            return {created.result(), created.data()};
        }
    }
    {
        // 'Note' table and data
        QSqlQuery q(db);
        const QString addAuthor =
                QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer"))
                .arg(TableNote::TBL_NAME).arg(TableNote::FLD_FK_AUTHOR);
        if (!q.exec(addAuthor)){
            return {false, q.lastError().text()};
        }

        const QString addTsCreate =
                QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer"))
                .arg(TableNote::TBL_NAME).arg(TableNote::FLD_TS_CREATE);
        if (!q.exec(addTsCreate)){
            return {false, q.lastError().text()};
        }
        const QString addTsEdit =
                QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer"))
                .arg(TableNote::TBL_NAME).arg(TableNote::FLD_TS_EDIT);
        if (!q.exec(addTsEdit)){
            return {false, q.lastError().text()};
        }

        const QString updateVer =
                QString(QStringLiteral("UPDATE %1 SET %2=%3, %4=%5"))
                .arg(TableNote::TBL_NAME)
                .arg(TableNote::FLD_TS_CREATE).arg(ts)
                .arg(TableNote::FLD_TS_EDIT).arg(ts);
        if (!q.exec(updateVer)){
            return {false, q.lastError().text()};
        }
        const QString updateAuth =
                QString(QStringLiteral("UPDATE %1 SET %2=%3 WHERE %4 is NULL"))
                .arg(TableNote::TBL_NAME)
                .arg(TableNote::FLD_FK_AUTHOR).arg(systemAuthorId)
                .arg(TableNote::FLD_FK_AUTHOR);
        if (!q.exec(updateAuth)){
            return {false, q.lastError().text()};
        }
    }
    {
        // 'TagsNotes' table
        QSqlQuery q(db);
        BoolResult_t created = TableTagsNotes::create(db);
        if (!created.result()){
            return {created.result(), created.data()};
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
            qInsert.addBindValue(1);
            if (!qInsert.exec()){
                return {false, qInsert.lastError().text()};
            }
        }

    }


    // Make commit or rollback
    if(res.result() == true){
        db.commit();
    }else{
        db.rollback();
    }

    // ~Transaction
    //------------------------------------------------------------------------------------------------------------------

    return res;
}

vfx_shared::BoolResult_t SqliteStorage::upgradeFromV2()
{
    DbHelper dbh(name());
    QSqlDatabase db = dbh.db();
    if(!db.isOpen()){
        return {false, dbh.lastError()};
    }

    //------------------------------------------------------------------------------------------------------------------
    // Transaction

    const bool ok = db.transaction();
    if(!ok) {
        return {false, tr("transaction not started", "DB")};
    }

    // if ok
    BoolResult_t res(true, "");
    qint64 ts = QDateTime::currentMSecsSinceEpoch();
    {
        // 'Config' table and data
        QSqlQuery q(db);
        const QString addTsCreate =
                QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer"))
                .arg(TableCfg::TBL_NAME).arg(TableCfg::FLD_TS_CREATE);
        if (!q.exec(addTsCreate)){
            return {false, q.lastError().text()};
        }
        const QString addTsEdit =
                QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer"))
                .arg(TableCfg::TBL_NAME).arg(TableCfg::FLD_TS_EDIT);
        if (!q.exec(addTsEdit)){
            return {false, q.lastError().text()};
        }

        const QString updateVer =
                QString(QStringLiteral("UPDATE %1 SET %2=%3, %4=%5, %6=%7 WHERE %8='%9'"))
                .arg(TableCfg::TBL_NAME)
                .arg(TableCfg::FLD_VALUE).arg(VERSION)
                .arg(TableCfg::FLD_TS_CREATE).arg(ts)
                .arg(TableCfg::FLD_TS_EDIT).arg(ts)
                .arg(TableCfg::FLD_KEY).arg("version");
        if (!q.exec(updateVer)){
            return {false, q.lastError().text()};
        }
    }
    qint32 defTagId = 0;
    {
        // 'Tag' table and default data
        BoolResult_t created = TableTags::create(db);
        if (!created.result()){
            return {created.result(), created.data()};
        }

        defTagId = created.data().toInt();
    }
    {
        // 'Note' table and data
        QSqlQuery q(db);
        const QString addTsCreate =
                QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer"))
                .arg(TableNote::TBL_NAME).arg(TableNote::FLD_TS_CREATE);
        if (!q.exec(addTsCreate)){
            return {false, q.lastError().text()};
        }
        const QString addTsEdit =
                QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer"))
                .arg(TableNote::TBL_NAME).arg(TableNote::FLD_TS_EDIT);
        if (!q.exec(addTsEdit)){
            return {false, q.lastError().text()};
        }

        const QString updateVer =
                QString(QStringLiteral("UPDATE %1 SET %2=%3, %4=%5"))
                .arg(TableNote::TBL_NAME)
                .arg(TableNote::FLD_TS_CREATE).arg(ts)
                .arg(TableNote::FLD_TS_EDIT).arg(ts);
        if (!q.exec(updateVer)){
            return {false, q.lastError().text()};
        }

    }
    {
        // 'Author' table and data
        QSqlQuery q(db);
        const QString addTsCreate =
                QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer"))
                .arg(TableAuthor::TBL_NAME).arg(TableAuthor::FLD_TS_CREATE);
        if (!q.exec(addTsCreate)){
            return {false, q.lastError().text()};
        }
        const QString addTsEdit =
                QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer"))
                .arg(TableAuthor::TBL_NAME).arg(TableAuthor::FLD_TS_EDIT);
        if (!q.exec(addTsEdit)){
            return {false, q.lastError().text()};
        }

        const QString updateVer =
                QString(QStringLiteral("UPDATE %1 SET %2=%3, %4=%5"))
                .arg(TableAuthor::TBL_NAME)
                .arg(TableAuthor::FLD_TS_CREATE).arg(ts)
                .arg(TableAuthor::FLD_TS_EDIT).arg(ts);
        if (!q.exec(updateVer)){
            return {false, q.lastError().text()};
        }
    }
    {
        // 'TagsNotes' table
        QSqlQuery q(db);
        BoolResult_t created = TableTagsNotes::create(db);
        if (!created.result()){
            return {created.result(), created.data()};
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
            qInsert.addBindValue(defTagId);
            if (!qInsert.exec()){
                return {false, qInsert.lastError().text()};
            }
        }

    }

    // Make commit or rollback
    if(res.result() == true){
        db.commit();
    }else{
        db.rollback();
    }

    // ~Transaction
    //------------------------------------------------------------------------------------------------------------------

    return res;
}

//QSqlDatabase SqliteStorage::getDb()
//{
//    QSqlDatabase db = QSqlDatabase::database();
//    if(!db.isOpen()) {
//        db = QSqlDatabase::addDatabase("QSQLITE");
//        db.setDatabaseName(name());
//    }
//    return db;
//}

