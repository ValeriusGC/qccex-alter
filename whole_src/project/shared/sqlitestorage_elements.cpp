#include <QSqlQuery>
#include <QSqlError>

#include "sqlitestorage_elements.h"
#include "shared_const.h"

namespace storage {

namespace sqlite {

//======================================================================================================================

const QString TableCfg::TBL_NAME        = QStringLiteral("tbl_config");

const QString TableCfg::FLD_ID          = QStringLiteral("id");
const QString TableCfg::FLD_KEY         = QStringLiteral("the_key");
const QString TableCfg::FLD_VALUE       = QStringLiteral("the_value");
const QString TableCfg::FLD_DESCR       = QStringLiteral("descr");
const QString TableCfg::FLD_TS_CREATE   = QStringLiteral("ts_create");
const QString TableCfg::FLD_TS_EDIT     = QStringLiteral("ts_edit");

TableCfg::TableCfg()
{
    INC_THIS(false);
}

TableCfg::~TableCfg()
{
    DEC_THIS(false);
}

BoolVariantResult_t TableCfg::doCreate(QSqlDatabase db)
{
    const QString QRY_CREATE =
            QString(QStringLiteral("create table %1(%2 integer primary key, %3 varchar, %4 varchar, "
                                   "%5 varchar, %6 integer, %7 integer)"))
            .arg(TBL_NAME).arg(FLD_ID).arg(FLD_KEY).arg(FLD_VALUE).arg(FLD_DESCR).arg(FLD_TS_CREATE).arg(FLD_TS_EDIT);
    QSqlQuery q(db);
    if (!q.exec(QRY_CREATE)){
        return {false, q.lastError().text()};
    }

    const QString QRY_INSERT(QStringLiteral("insert into %1(%2, %3) values(?, ?)")
                             .arg(TBL_NAME).arg(FLD_KEY).arg(FLD_VALUE));
    if (!q.prepare(QRY_INSERT)){
        return {false, q.lastError().text()};
    }
    q.addBindValue("version");
    q.addBindValue(NEW_VERSION);
    if (!q.exec()){
        return {false, q.lastError().text()};
    }

    return RES_TRUE;
}

BoolVariantResult_t TableCfg::doUpgrade(QSqlDatabase db, const V1_t &fromVersion)
{
    QSqlQuery q(db);
    const QString addDescr =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 varchar")).arg(TBL_NAME).arg(FLD_DESCR);
    if (!q.exec(addDescr)){
        return {false, q.lastError().text()};
    }
    const QString addTsCreate =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer")).arg(TBL_NAME).arg(FLD_TS_CREATE);
    if (!q.exec(addTsCreate)){
        return {false, q.lastError().text()};
    }
    const QString addTsEdit =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer")).arg(TBL_NAME).arg(FLD_TS_EDIT);
    if (!q.exec(addTsEdit)){
        return {false, q.lastError().text()};
    }

    const qint64 ts = QDateTime::currentMSecsSinceEpoch();
    const QString updateVer =
            QString(QStringLiteral("UPDATE %1 SET %2=%3, %4=%5, %6=%7 WHERE %8='%9'"))
            .arg(TBL_NAME)
            .arg(FLD_VALUE).arg(NEW_VERSION)
            .arg(FLD_TS_CREATE).arg(ts)
            .arg(FLD_TS_EDIT).arg(ts)
            .arg(FLD_KEY).arg("version");
    if (!q.exec(updateVer)){
        return {false, q.lastError().text()};
    }

    return {true, fromVersion.value};
}

BoolVariantResult_t TableCfg::doUpgrade(QSqlDatabase db, const V2_t &fromVersion)
{
    QSqlQuery q(db);
    const QString addTsCreate =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer")).arg(TBL_NAME).arg(FLD_TS_CREATE);
    if (!q.exec(addTsCreate)){
        return {false, q.lastError().text()};
    }
    const QString addTsEdit =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer")).arg(TBL_NAME).arg(FLD_TS_EDIT);
    if (!q.exec(addTsEdit)){
        return {false, q.lastError().text()};
    }

    const qint64 ts = QDateTime::currentMSecsSinceEpoch();
    const QString updateVer =
            QString(QStringLiteral("UPDATE %1 SET %2=%3, %4=%5, %6=%7 WHERE %8='%9'"))
            .arg(TableCfg::TBL_NAME)
            .arg(TableCfg::FLD_VALUE).arg(NEW_VERSION)
            .arg(TableCfg::FLD_TS_CREATE).arg(ts)
            .arg(TableCfg::FLD_TS_EDIT).arg(ts)
            .arg(TableCfg::FLD_KEY).arg("version");
    if (!q.exec(updateVer)){
        return {false, q.lastError().text()};
    }

    return {true, fromVersion.value};
}

//BoolResult_t TableCfg::doUpgradeFromV1(QSqlDatabase db)
//{
//    QSqlQuery q(db);
//    const QString addDescr =
//            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 varchar")).arg(TBL_NAME).arg(FLD_DESCR);
//    if (!q.exec(addDescr)){
//        return {false, q.lastError().text()};
//    }
//    const QString addTsCreate =
//            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer")).arg(TBL_NAME).arg(FLD_TS_CREATE);
//    if (!q.exec(addTsCreate)){
//        return {false, q.lastError().text()};
//    }
//    const QString addTsEdit =
//            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer")).arg(TBL_NAME).arg(FLD_TS_EDIT);
//    if (!q.exec(addTsEdit)){
//        return {false, q.lastError().text()};
//    }

//    const qint64 ts = QDateTime::currentMSecsSinceEpoch();
//    const QString updateVer =
//            QString(QStringLiteral("UPDATE %1 SET %2=%3, %4=%5, %6=%7 WHERE %8='%9'"))
//            .arg(TBL_NAME)
//            .arg(FLD_VALUE).arg(NEW_VERSION)
//            .arg(FLD_TS_CREATE).arg(ts)
//            .arg(FLD_TS_EDIT).arg(ts)
//            .arg(FLD_KEY).arg("version");
//    if (!q.exec(updateVer)){
//        return {false, q.lastError().text()};
//    }

//    return RES_TRUE;
//}

//BoolResult_t TableCfg::doUpgradeFromV2(QSqlDatabase db)
//{
//    QSqlQuery q(db);
//    const QString addTsCreate =
//            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer")).arg(TBL_NAME).arg(FLD_TS_CREATE);
//    if (!q.exec(addTsCreate)){
//        return {false, q.lastError().text()};
//    }
//    const QString addTsEdit =
//            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer")).arg(TBL_NAME).arg(FLD_TS_EDIT);
//    if (!q.exec(addTsEdit)){
//        return {false, q.lastError().text()};
//    }

//    const qint64 ts = QDateTime::currentMSecsSinceEpoch();
//    const QString updateVer =
//            QString(QStringLiteral("UPDATE %1 SET %2=%3, %4=%5, %6=%7 WHERE %8='%9'"))
//            .arg(TableCfg::TBL_NAME)
//            .arg(TableCfg::FLD_VALUE).arg(NEW_VERSION)
//            .arg(TableCfg::FLD_TS_CREATE).arg(ts)
//            .arg(TableCfg::FLD_TS_EDIT).arg(ts)
//            .arg(TableCfg::FLD_KEY).arg("version");
//    if (!q.exec(updateVer)){
//        return {false, q.lastError().text()};
//    }

//    return RES_TRUE;
//}

//======================================================================================================================

const QString TableNote::TBL_NAME       = QStringLiteral("tbl_notes");

const QString TableNote::FLD_ID         = QStringLiteral("id");
const QString TableNote::FLD_TEXT       = QStringLiteral("the_text");
const QString TableNote::FLD_FK_AUTHOR  = QStringLiteral("fk_author");
const QString TableNote::FLD_TS_CREATE  = QStringLiteral("ts_create");
const QString TableNote::FLD_TS_EDIT    = QStringLiteral("ts_edit");
const QString TableNote::FLD_DEL        = QStringLiteral("deleted");

BoolVariantResult_t TableNote::doCreate(QSqlDatabase db)
{
    const QString QRY_CREATE =
            QString(QStringLiteral("create table %1(%2 integer primary key, %3 integer, %4 varchar, %5 integer, "
                                   "%6 integer, %7 integer)"))
            .arg(TBL_NAME).arg(FLD_ID).arg(FLD_FK_AUTHOR).arg(FLD_TEXT).arg(FLD_TS_CREATE)
            .arg(FLD_TS_EDIT).arg(FLD_DEL);
    QSqlQuery q(db);
    if (!q.exec(QRY_CREATE)){
        return {false, q.lastError().text()};
    }

    return {true, QVariant()};
}

BoolVariantResult_t TableNote::doUpgrade(QSqlDatabase db, const V1_t &fromVersion)
{
    QSqlQuery q(db);
    const QString addAuthor =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer")).arg(TBL_NAME).arg(FLD_FK_AUTHOR);
    if (!q.exec(addAuthor)){
        return {false, q.lastError().text()};
    }

    const QString addTsCreate =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer")).arg(TBL_NAME).arg(FLD_TS_CREATE);
    if (!q.exec(addTsCreate)){
        return {false, q.lastError().text()};
    }
    const QString addTsEdit =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer")).arg(TBL_NAME).arg(FLD_TS_EDIT);
    if (!q.exec(addTsEdit)){
        return {false, q.lastError().text()};
    }
    const QString addDel =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer DEFAULT 0"))
            .arg(TBL_NAME).arg(FLD_DEL);
    if (!q.exec(addDel)){
        return {false, q.lastError().text()};
    }

    const qint64 ts = QDateTime::currentMSecsSinceEpoch();
    const QString updateVer =
            QString(QStringLiteral("UPDATE %1 SET %2=%3, %4=%5"))
            .arg(TBL_NAME)
            .arg(FLD_TS_CREATE).arg(ts)
            .arg(FLD_TS_EDIT).arg(ts);
    if (!q.exec(updateVer)){
        return {false, q.lastError().text()};
    }

    return {true, fromVersion.value};
}

BoolVariantResult_t TableNote::doUpgrade(QSqlDatabase db, const V2_t &fromVersion)
{
    QSqlQuery q(db);
    const QString addTsCreate =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer")).arg(TBL_NAME).arg(FLD_TS_CREATE);
    if (!q.exec(addTsCreate)){
        return {false, q.lastError().text()};
    }
    const QString addTsEdit =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer")).arg(TBL_NAME).arg(FLD_TS_EDIT);
    if (!q.exec(addTsEdit)){
        return {false, q.lastError().text()};
    }
    const QString addDel =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer DEFAULT 0"))
            .arg(TBL_NAME).arg(FLD_DEL);
    if (!q.exec(addDel)){
        return {false, q.lastError().text()};
    }

    const qint64 ts = QDateTime::currentMSecsSinceEpoch();
    const QString updateVer =
            QString(QStringLiteral("UPDATE %1 SET %2=%3, %4=%5"))
            .arg(TBL_NAME)
            .arg(FLD_TS_CREATE).arg(ts)
            .arg(FLD_TS_EDIT).arg(ts);
    if (!q.exec(updateVer)){
        return {false, q.lastError().text()};
    }

    return {true, fromVersion.value};
}

//BoolResult_t TableNote::doUpgradeFromV1(QSqlDatabase db)
//{
//    QSqlQuery q(db);
//    const QString addAuthor =
//            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer")).arg(TBL_NAME).arg(FLD_FK_AUTHOR);
//    if (!q.exec(addAuthor)){
//        return {false, q.lastError().text()};
//    }

//    const QString addTsCreate =
//            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer")).arg(TBL_NAME).arg(FLD_TS_CREATE);
//    if (!q.exec(addTsCreate)){
//        return {false, q.lastError().text()};
//    }
//    const QString addTsEdit =
//            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer")).arg(TBL_NAME).arg(FLD_TS_EDIT);
//    if (!q.exec(addTsEdit)){
//        return {false, q.lastError().text()};
//    }
//    const QString addDel =
//            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer DEFAULT 0"))
//            .arg(TBL_NAME).arg(FLD_DEL);
//    if (!q.exec(addDel)){
//        return {false, q.lastError().text()};
//    }

//    const qint64 ts = QDateTime::currentMSecsSinceEpoch();
//    const QString updateVer =
//            QString(QStringLiteral("UPDATE %1 SET %2=%3, %4=%5"))
//            .arg(TBL_NAME)
//            .arg(FLD_TS_CREATE).arg(ts)
//            .arg(FLD_TS_EDIT).arg(ts);
//    if (!q.exec(updateVer)){
//        return {false, q.lastError().text()};
//    }

//    return {true, ""};
//}

//BoolResult_t TableNote::doUpgradeFromV2(QSqlDatabase db)
//{
//    QSqlQuery q(db);
//    const QString addTsCreate =
//            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer")).arg(TBL_NAME).arg(FLD_TS_CREATE);
//    if (!q.exec(addTsCreate)){
//        return {false, q.lastError().text()};
//    }
//    const QString addTsEdit =
//            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer")).arg(TBL_NAME).arg(FLD_TS_EDIT);
//    if (!q.exec(addTsEdit)){
//        return {false, q.lastError().text()};
//    }
//    const QString addDel =
//            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer DEFAULT 0"))
//            .arg(TBL_NAME).arg(FLD_DEL);
//    if (!q.exec(addDel)){
//        return {false, q.lastError().text()};
//    }

//    const qint64 ts = QDateTime::currentMSecsSinceEpoch();
//    const QString updateVer =
//            QString(QStringLiteral("UPDATE %1 SET %2=%3, %4=%5"))
//            .arg(TBL_NAME)
//            .arg(FLD_TS_CREATE).arg(ts)
//            .arg(FLD_TS_EDIT).arg(ts);
//    if (!q.exec(updateVer)){
//        return {false, q.lastError().text()};
//    }
//    return {true, ""};
//}

//======================================================================================================================

const QString TableAuthor::TBL_NAME         = QStringLiteral("tbl_authors");

const QString TableAuthor::FLD_ID           = QStringLiteral("id");
const QString TableAuthor::FLD_TITLE        = QStringLiteral("title");
const QString TableAuthor::FLD_TS_CREATE    = QStringLiteral("ts_create");
const QString TableAuthor::FLD_TS_EDIT      = QStringLiteral("ts_edit");
const QString TableAuthor::FLD_DEL          = QStringLiteral("deleted");

//vfx_shared::BoolResult_t TableAuthor::create(QSqlDatabase db)
//{
//    const QString QRY_CREATE =
//            QString(QStringLiteral("create table %1(%2 integer primary key, %3 varchar, %4 integer, "
//                                   "%5 integer, %6 integer)"))
//            .arg(TBL_NAME).arg(FLD_ID).arg(FLD_TITLE).arg(FLD_TS_CREATE).arg(FLD_TS_EDIT).arg(FLD_DEL);

//    QSqlQuery q(db);
//    if (!q.exec(QRY_CREATE)){
//        return {false, q.lastError().text()};
//    }

//    // 'System author': default value for "orphan" notes.
//    const QString QRY_INSERT(QStringLiteral("insert into %1(%2, %3, %4, %5) values(?, ?, ?, ?)")
//                             .arg(TBL_NAME).arg(FLD_TITLE).arg(FLD_TS_CREATE).arg(FLD_TS_EDIT).arg(FLD_DEL));
//    if (!q.prepare(QRY_INSERT)){
//        return {false, q.lastError().text()};
//    }systemAuthorId
//    qint64 ts = QDateTime::currentMSecsSinceEpoch();
//    q.addBindValue(QObject::tr("System user", "DB"));
//    q.addBindValue(ts);
//    q.addBindValue(ts);
//    q.addBindValue(0);
//    if (!q.exec()){
//        return {false, q.lastError().text()};
//    }
//    return {true, q.lastInsertId().toString()};
//}

BoolVariantResult_t TableAuthor::doCreate(QSqlDatabase db)
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
    const qint64 ts = QDateTime::currentMSecsSinceEpoch();
    q.addBindValue(model::Constants::DEFAULT_AUTHOR);
    q.addBindValue(ts);
    q.addBindValue(ts);
    q.addBindValue(0);
    if (!q.exec()){
        return {false, q.lastError().text()};
    }
    return {true, q.lastInsertId().toString()};
}

BoolVariantResult_t TableAuthor::doUpgrade(QSqlDatabase db, const V1_t &fromVersion)
{
    Q_UNUSED(fromVersion);
    return doCreate(db);
}

BoolVariantResult_t TableAuthor::doUpgrade(QSqlDatabase db, const V2_t &fromVersion)
{
    // 'Author' table and data
    QSqlQuery q(db);
    const QString addTsCreate =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer")).arg(TBL_NAME).arg(FLD_TS_CREATE);
    if (!q.exec(addTsCreate)){
        return {false, q.lastError().text()};
    }
    const QString addTsEdit =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer")).arg(TBL_NAME).arg(FLD_TS_EDIT);
    if (!q.exec(addTsEdit)){
        return {false, q.lastError().text()};
    }

    qint64 ts = QDateTime::currentMSecsSinceEpoch();
    const QString updateVer =
            QString(QStringLiteral("UPDATE %1 SET %2=%3, %4=%5"))
            .arg(TBL_NAME)
            .arg(FLD_TS_CREATE).arg(ts)
            .arg(FLD_TS_EDIT).arg(ts);
    if (!q.exec(updateVer)){
        return {false, q.lastError().text()};
    }

    return {true, fromVersion.value};
}

//BoolResult_t TableAuthor::doUpgradeFromV1(QSqlDatabase db)
//{
//    return doCreate(db);
//}

//BoolResult_t TableAuthor::doUpgradeFromV2(QSqlDatabase db)
//{
//    // 'Author' table and data
//    QSqlQuery q(db);
//    const QString addTsCreate =
//            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer")).arg(TBL_NAME).arg(FLD_TS_CREATE);
//    if (!q.exec(addTsCreate)){
//        return {false, q.lastError().text()};
//    }
//    const QString addTsEdit =
//            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 integer")).arg(TBL_NAME).arg(FLD_TS_EDIT);
//    if (!q.exec(addTsEdit)){
//        return {false, q.lastError().text()};
//    }

//    qint64 ts = QDateTime::currentMSecsSinceEpoch();
//    const QString updateVer =
//            QString(QStringLiteral("UPDATE %1 SET %2=%3, %4=%5"))
//            .arg(TBL_NAME)
//            .arg(FLD_TS_CREATE).arg(ts)
//            .arg(FLD_TS_EDIT).arg(ts);
//    if (!q.exec(updateVer)){
//        return {false, q.lastError().text()};
//    }

//    return {true, ""};
//}



//======================================================================================================================

const QString TableTags::TBL_NAME       = QStringLiteral("tbl_tags");

const QString TableTags::FLD_ID         = QStringLiteral("id");
const QString TableTags::FLD_TEXT       = QStringLiteral("title");
const QString TableTags::FLD_DEL        = QStringLiteral("deleted");

BoolVariantResult_t TableTags::doCreate(QSqlDatabase db)
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
    q.addBindValue(model::Constants::TAG_INCOMING);
    q.addBindValue(0);
    if (!q.exec()){
        return {false, q.lastError().text()};
    }
    return {true, q.lastInsertId().toString()};
}

BoolVariantResult_t TableTags::doUpgrade(QSqlDatabase db, const V1_t &fromVersion)
{
    Q_UNUSED(fromVersion);
    return doCreate(db);
}

BoolVariantResult_t TableTags::doUpgrade(QSqlDatabase db, const V2_t &fromVersion)
{
    Q_UNUSED(fromVersion);
    return doCreate(db);
}

//BoolResult_t TableTags::doUpgradeFromV1(QSqlDatabase db)
//{
//    return doCreate(db);
//}

//BoolResult_t TableTags::doUpgradeFromV2(QSqlDatabase db)
//{
//    return doCreate(db);
//}

//======================================================================================================================

const QString TableTagsNotes::TBL_NAME      = QStringLiteral("tbl_tags_notes");

const QString TableTagsNotes::FLD_ID        = QStringLiteral("id");
const QString TableTagsNotes::FLD_FK_TAGS   = QStringLiteral("fk_tags");
const QString TableTagsNotes::FLD_FK_NOTES  = QStringLiteral("fk_notes");

BoolVariantResult_t TableTagsNotes::doCreate(QSqlDatabase db)
{
    const QString QRY_CREATE =
            QString(QStringLiteral("create table %1(%2 integer primary key, %3 varchar, %4 integer)"))
            .arg(TBL_NAME).arg(FLD_ID).arg(FLD_FK_TAGS).arg(FLD_FK_NOTES);

    QSqlQuery q(db);
    if (!q.exec(QRY_CREATE)){
        return {false, q.lastError().text()};
    }

    return {true, QVariant()};
}

BoolVariantResult_t TableTagsNotes::doUpgrade(QSqlDatabase db, const V1_t &fromVersion)
{
    Q_UNUSED(fromVersion);
    return doCreate(db);
}

BoolVariantResult_t TableTagsNotes::doUpgrade(QSqlDatabase db, const V2_t &fromVersion)
{
    Q_UNUSED(fromVersion);
    return doCreate(db);
}

//BoolResult_t TableTagsNotes::doUpgradeFromV1(QSqlDatabase db)
//{
//    return doCreate(db);
//}

//BoolResult_t TableTagsNotes::doUpgradeFromV2(QSqlDatabase db)
//{
//    return doCreate(db);
//}

//======================================================================================================================

} // namespace sqlite

} // namespace storage
