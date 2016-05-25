#include <QSqlQuery>
#include <QSqlError>
#include "sqlite_storage_v3.h"

namespace storage {

namespace sqlite {

namespace v3 {

const qint32 VERSION = 3;

//======================================================================================================================

const QString TableCfg::TBL_NAME        = QStringLiteral("tbl_config");

const QString TableCfg::FLD_ID          = QStringLiteral("id");
const QString TableCfg::FLD_KEY         = QStringLiteral("the_key");
const QString TableCfg::FLD_VALUE       = QStringLiteral("the_value");
const QString TableCfg::FLD_DESCR       = QStringLiteral("descr");
const QString TableCfg::FLD_TS_CREATE   = QStringLiteral("ts_create");
const QString TableCfg::FLD_TS_EDIT     = QStringLiteral("ts_edit");

const QString TableCfg::QRY_CREATE =
        QString(QStringLiteral("create table %1(%2 integer primary key, %3 varchar, %4 varchar, "
                               "%5 varchar, %6 integer, %7 integer)"))
        .arg(TableCfg::TBL_NAME).arg(TableCfg::FLD_ID).arg(TableCfg::FLD_KEY)
        .arg(TableCfg::FLD_VALUE).arg(TableCfg::FLD_DESCR)
        .arg(TableCfg::FLD_TS_CREATE).arg(TableCfg::FLD_TS_EDIT);

//======================================================================================================================

const QString TableNote::TBL_NAME       = QStringLiteral("tbl_notes");

const QString TableNote::FLD_ID         = QStringLiteral("id");
const QString TableNote::FLD_TEXT       = QStringLiteral("the_text");
const QString TableNote::FLD_FK_AUTHOR  = QStringLiteral("fk_author");
const QString TableNote::FLD_TS_CREATE  = QStringLiteral("ts_create");
const QString TableNote::FLD_TS_EDIT    = QStringLiteral("ts_edit");
const QString TableNote::FLD_DEL        = QStringLiteral("deleted");

const QString TableNote::QRY_CREATE =
        QString(QStringLiteral("create table %1(%2 integer primary key, %3 integer, %4 varchar, %5 integer, "
                               "%6 integer, %7 integer)"))
        .arg(TableNote::TBL_NAME).arg(TableNote::FLD_ID).arg(TableNote::FLD_FK_AUTHOR).arg(TableNote::FLD_TEXT)
        .arg(TableNote::FLD_TS_CREATE).arg(TableNote::FLD_TS_EDIT).arg(TableNote::FLD_DEL);

//======================================================================================================================

const QString TableAuthor::TBL_NAME         = QStringLiteral("tbl_authors");

const QString TableAuthor::FLD_ID           = QStringLiteral("id");
const QString TableAuthor::FLD_TITLE        = QStringLiteral("title");
const QString TableAuthor::FLD_TS_CREATE    = QStringLiteral("ts_create");
const QString TableAuthor::FLD_TS_EDIT      = QStringLiteral("ts_edit");
const QString TableAuthor::FLD_DEL          = QStringLiteral("deleted");

vfx_shared::BoolResult_t TableAuthor::create(QSqlDatabase db)
{
    const QString QRY_CREATE =
            QString(QStringLiteral("create table %1(%2 integer primary key, %3 varchar, %4 integer, "
                                   "%5 integer, %6 integer)"))
            .arg(TBL_NAME).arg(FLD_ID).arg(FLD_TITLE).arg(FLD_TS_CREATE).arg(FLD_TS_EDIT).arg(FLD_DEL);

    QSqlQuery q(db);
    if (!q.exec(QRY_CREATE)){
        return {false, q.lastError().text()};
    }

    // 'System author': default value for "orphan" notes.
    const QString QRY_INSERT(QStringLiteral("insert into %1(%2, %3, %4, %5) values(?, ?, ?, ?)")
                             .arg(TBL_NAME).arg(FLD_TITLE).arg(FLD_TS_CREATE).arg(FLD_TS_EDIT).arg(FLD_DEL));
    if (!q.prepare(QRY_INSERT)){
        return {false, q.lastError().text()};
    }
    qint64 ts = QDateTime::currentMSecsSinceEpoch();
    q.addBindValue(QObject::tr("System user", "DB"));
    q.addBindValue(ts);
    q.addBindValue(ts);
    q.addBindValue(0);
    if (!q.exec()){
        return {false, q.lastError().text()};
    }
    return {true, q.lastInsertId().toString()};
}

//======================================================================================================================

const QString TableTags::TBL_NAME       = QStringLiteral("tbl_tags");

const QString TableTags::FLD_ID         = QStringLiteral("id");
const QString TableTags::FLD_TEXT       = QStringLiteral("title");
const QString TableTags::FLD_DEL        = QStringLiteral("deleted");

vfx_shared::BoolResult_t TableTags::create(QSqlDatabase db)
{
    const QString QRY_CREATE =
            QString(QStringLiteral("create table %1(%2 integer primary key, %3 varchar, %4 integer)"))
            .arg(TBL_NAME).arg(FLD_ID).arg(FLD_TEXT).arg(FLD_DEL);

    QSqlQuery q(db);
    if (!q.exec(QRY_CREATE)){
        return {false, q.lastError().text()};
    }

    // 'Default tag': default value for notes without tags.
    const QString QRY_INSERT(QStringLiteral("insert into %1(%2, %3) values(?, ?)")
                             .arg(TBL_NAME).arg(FLD_TEXT).arg(FLD_DEL));
    if (!q.prepare(QRY_INSERT)){
        return {false, q.lastError().text()};
    }
    q.addBindValue(QObject::tr("Incoming", "DB"));
    q.addBindValue(0);
    if (!q.exec()){
        return {false, q.lastError().text()};
    }
    return {true, q.lastInsertId().toString()};
}

//======================================================================================================================

const QString TableTagsNotes::TBL_NAME      = QStringLiteral("tbl_tags_notes");

const QString TableTagsNotes::FLD_ID        = QStringLiteral("id");
const QString TableTagsNotes::FLD_FK_TAGS   = QStringLiteral("fk_tags");
const QString TableTagsNotes::FLD_FK_NOTES  = QStringLiteral("fk_notes");

vfx_shared::BoolResult_t TableTagsNotes::create(QSqlDatabase db)
{
    const QString QRY_CREATE =
            QString(QStringLiteral("create table %1(%2 integer primary key, %3 varchar, %4 integer)"))
            .arg(TBL_NAME).arg(FLD_ID).arg(FLD_FK_TAGS).arg(FLD_FK_NOTES);

    QSqlQuery q(db);
    if (!q.exec(QRY_CREATE)){
        return {false, q.lastError().text()};
    }

    return {true, ""};
}

//======================================================================================================================


} // namespace v2

} // namespace sqlite

} // namespace storage
