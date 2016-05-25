#include <QSqlQuery>
#include <QSqlError>

#include "sqlite_storage_v2.h"

//const qint32 storage::sqlite::v2::VERSION = 2;

namespace storage {

namespace sqlite {

namespace v2 {

const qint32 VERSION = 2;

const QString TableCfg::TBL_NAME        = QStringLiteral("tbl_config");
const QString TableCfg::FLD_ID          = QStringLiteral("id");
const QString TableCfg::FLD_KEY         = QStringLiteral("the_key");
const QString TableCfg::FLD_VALUE       = QStringLiteral("the_value");
const QString TableCfg::FLD_DESCR       = QStringLiteral("descr");
const QString TableCfg::QRY_CREATE      =
        QString(QStringLiteral("create table %1(%2 integer primary key, %3 varchar, %4 varchar, %5 varchar)"))
        .arg(TableCfg::TBL_NAME).arg(TableCfg::FLD_ID).arg(TableCfg::FLD_KEY)
        .arg(TableCfg::FLD_VALUE).arg(TableCfg::FLD_DESCR);

const QString TableNote::TBL_NAME       = QStringLiteral("tbl_notes");
const QString TableNote::FLD_ID         = QStringLiteral("id");
const QString TableNote::FLD_TEXT       = QStringLiteral("the_text");
const QString TableNote::FLD_FK_AUTHOR  = QStringLiteral("fk_author");
const QString TableNote::QRY_CREATE     =
        QString(QStringLiteral("create table %1(%2 integer primary key, %3 integer, %4 varchar)"))
        .arg(TableNote::TBL_NAME).arg(TableNote::FLD_ID).arg(TableNote::FLD_FK_AUTHOR).arg(TableNote::FLD_TEXT);

const QString TableAuthor::TBL_NAME     = QStringLiteral("tbl_authors");
const QString TableAuthor::FLD_ID       = QStringLiteral("id");
const QString TableAuthor::FLD_TITLE    = QStringLiteral("title");

vfx_shared::BoolResult_t TableAuthor::create(QSqlDatabase db)
{
    const QString QRY_CREATE      =
            QString(QStringLiteral("create table %1(%2 integer primary key, %3 varchar)"))
            .arg(TableAuthor::TBL_NAME).arg(TableAuthor::FLD_ID).arg(TableAuthor::FLD_TITLE);

    QSqlQuery q(db);
    if (!q.exec(QRY_CREATE)){
        return {false, q.lastError().text()};
    }

    // 'System author': default value for "orphan" notes.
    const QString QRY_INSERT(QStringLiteral("insert into %1(%2) values(?)")
                             .arg(TableAuthor::TBL_NAME).arg(TableAuthor::FLD_TITLE));

    if (!q.prepare(QRY_INSERT)){
        return {false, q.lastError().text()};
    }
    q.addBindValue(QObject::tr("System user", "DB"));
    if (!q.exec()){
        return {false, q.lastError().text()};
    }
    return {true, q.lastInsertId().toString()};
}


} // namespace v2

} // namespace sqlite

} // namespace storage
