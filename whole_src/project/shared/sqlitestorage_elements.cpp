#include <QSqlQuery>
#include <QSqlError>
#include <QUuid>

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
    INC_THIS(true);
}

TableCfg::~TableCfg()
{
    DEC_THIS(true);
}

BoolVariantResult_t TableCfg::doCreate(const SqlEngine_t &engine)
{
    const QString QRY_CREATE =
            QString(QStringLiteral("CREATE TABLE %1(%2 INTEGER PRIMARY KEY ASC, %3 VARCHAR, %4 VARCHAR, "
                                   "%5 VARCHAR, %6 INTEGER, %7 INTEGER)"))
            .arg(TBL_NAME).arg(FLD_ID)
            .arg(FLD_KEY).arg(FLD_VALUE).arg(FLD_DESCR).arg(FLD_TS_CREATE).arg(FLD_TS_EDIT);
    QSqlQuery q(engine->db());
    if (!q.exec(QRY_CREATE)){
        return {false, q.lastError().text()};
    }

    const QString QRY_INSERT(QStringLiteral("INSERT INTO %1(%2, %3, %4, %5, %6) "
                                            "VALUES(:key, :value, :descr, :ts_create, :ts_edit)")
                             .arg(TBL_NAME).arg(FLD_KEY).arg(FLD_VALUE).arg(FLD_DESCR)
                             .arg(FLD_TS_CREATE).arg(FLD_TS_EDIT));
    if (!q.prepare(QRY_INSERT)){
        return {false, q.lastError().text()};
    }
    const qint64 ts = QDateTime::currentMSecsSinceEpoch();
    q.bindValue(":key", "version");
    q.bindValue(":value", NEW_VERSION);
    q.bindValue(":descr", QObject::tr("Storage version", "DB"));
    q.bindValue(":ts_create", ts);
    q.bindValue(":ts_edit", ts);
    if (!q.exec()){
        return {false, q.lastError().text()};
    }

    return RES_TRUE;
}

BoolVariantResult_t TableCfg::doUpgrade(const SqlEngine_t &engine, const V1_t &fromVersion)
{
    Q_UNUSED(fromVersion);
    QSqlQuery q(engine->db());
    const QString addDescr =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 VARCHAR")).arg(TBL_NAME).arg(FLD_DESCR);
    if (!q.exec(addDescr)){
        return {false, q.lastError().text()};
    }
    const QString addTsCreate =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 INTEGER")).arg(TBL_NAME).arg(FLD_TS_CREATE);
    if (!q.exec(addTsCreate)){
        return {false, q.lastError().text()};
    }
    const QString addTsEdit =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 INTEGER")).arg(TBL_NAME).arg(FLD_TS_EDIT);
    if (!q.exec(addTsEdit)){
        return {false, q.lastError().text()};
    }

    return updateVersion(engine);
}

BoolVariantResult_t TableCfg::doUpgrade(const SqlEngine_t &engine, const V2_t &fromVersion)
{
    Q_UNUSED(fromVersion);
    QSqlQuery q(engine->db());
    const QString addTsCreate =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 INTEGER")).arg(TBL_NAME).arg(FLD_TS_CREATE);
    if (!q.exec(addTsCreate)){
        return {false, q.lastError().text()};
    }
    const QString addTsEdit =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 INTEGER")).arg(TBL_NAME).arg(FLD_TS_EDIT);
    if (!q.exec(addTsEdit)){
        return {false, q.lastError().text()};
    }

    return updateVersion(engine);
}

BoolVariantResult_t TableCfg::doUpgrade(const SqlEngine_t &engine, const V3_t &fromVersion)
{
    Q_UNUSED(fromVersion);
    return updateVersion(engine);
}

BoolVariantResult_t TableCfg::updateVersion(SqlEngineSharedPtr_t enginePtr)
{
    const qint64 ts = QDateTime::currentMSecsSinceEpoch();
    const QString updateVer =
            QString(QStringLiteral("UPDATE %1 SET %2=%3, %4=%5 WHERE %6='%7'"))
            .arg(TBL_NAME)
            .arg(FLD_VALUE).arg(NEW_VERSION)
            .arg(FLD_TS_EDIT).arg(ts)
            .arg(FLD_KEY).arg("version");
    QSqlQuery q(enginePtr->db());
    if (!q.exec(updateVer)){
        return {false, q.lastError().text()};
    }
    return {true, QVariant()};
}

//======================================================================================================================

const QString TableNote::TBL_NAME       = QStringLiteral("tbl_notes");
const QString TableNote::FLD_ID         = QStringLiteral("id");
const QString TableNote::FLD_UUID       = QStringLiteral("uuid");
const QString TableNote::FLD_TEXT       = QStringLiteral("the_text");
const QString TableNote::FLD_FK_AUTHOR  = QStringLiteral("fk_author");
const QString TableNote::FLD_TS_CREATE  = QStringLiteral("ts_create");
const QString TableNote::FLD_TS_EDIT    = QStringLiteral("ts_edit");
const QString TableNote::FLD_DEL        = QStringLiteral("deleted");

BoolVariantResult_t TableNote::doCreate(const SqlEngine_t &engine)
{
    const QString QRY_CREATE =
            QString(QStringLiteral("CREATE TABLE %1(%2 INTEGER PRIMARY KEY ASC, %3 VARCHAR(24) NOT NULL, "
                                   "%4 INTEGER, %5 VARCHAR, %6 INTEGER, "
                                   "%7 INTEGER, %8 INTEGER,"
                                   "CONSTRAINT `id_UNIQUE` UNIQUE (`%3`)"
                                   ")"))
            .arg(TBL_NAME).arg(FLD_ID).arg(FLD_UUID)
            .arg(FLD_FK_AUTHOR).arg(FLD_TEXT).arg(FLD_TS_CREATE)
            .arg(FLD_TS_EDIT).arg(FLD_DEL);
    QSqlQuery q(engine->db());
    if (!q.exec(QRY_CREATE)){
        return {false, q.lastError().text()};
    }

    return {true, QVariant()};
}

BoolVariantResult_t TableNote::doUpgrade(const SqlEngine_t &engine, const V1_t &fromVersion)
{
    QSqlQuery q(engine->db());
    const QString addUuid =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 VARCHAR(24) NOT NULL DEFAULT \"\""))
            .arg(TBL_NAME).arg(FLD_UUID);
    if (!q.exec(addUuid)){
        return {false, q.lastError().text()};
    }

    // Before unique index we should create unique values
    BoolVariantResult_t created = createUuids(engine);
    if(!created.result()) {
        return created;
    }

    //
    const QString addUuidIndex =
            QString(QStringLiteral("CREATE UNIQUE INDEX IF NOT EXISTS id_UNIQUE ON %1 (%2)"))
            .arg(TBL_NAME).arg(FLD_UUID);
    if (!q.exec(addUuidIndex)){
        return {false, q.lastError().text()};
    }

    //
    const QString addAuthor =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 INTEGER")).arg(TBL_NAME).arg(FLD_FK_AUTHOR);
    if (!q.exec(addAuthor)){
        return {false, q.lastError().text()};
    }

    const QString addTsCreate =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 INTEGER")).arg(TBL_NAME).arg(FLD_TS_CREATE);
    if (!q.exec(addTsCreate)){
        return {false, q.lastError().text()};
    }
    const QString addTsEdit =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 INTEGER")).arg(TBL_NAME).arg(FLD_TS_EDIT);
    if (!q.exec(addTsEdit)){
        return {false, q.lastError().text()};
    }
    const QString addDel =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 INTEGER DEFAULT 0"))
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

BoolVariantResult_t TableNote::doUpgrade(const SqlEngine_t &engine, const V2_t &fromVersion)
{
    QSqlQuery q(engine->db());
    const QString addUuid =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 VARCHAR(24) NOT NULL DEFAULT \"\""))
            .arg(TBL_NAME).arg(FLD_UUID);
    if (!q.exec(addUuid)){
        return {false, q.lastError().text()};
    }

    // Before unique index we should create unique values
    BoolVariantResult_t created = createUuids(engine);
    if(!created.result()) {
        return created;
    }

    const QString addUuidIndex =
            QString(QStringLiteral("CREATE UNIQUE INDEX IF NOT EXISTS id_UNIQUE ON %1 (%2)"))
            .arg(TBL_NAME).arg(FLD_UUID);
    if (!q.exec(addUuidIndex)){
        return {false, q.lastError().text()};
    }
    const QString addTsCreate =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 INTEGER")).arg(TBL_NAME).arg(FLD_TS_CREATE);
    if (!q.exec(addTsCreate)){
        return {false, q.lastError().text()};
    }
    const QString addTsEdit =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 INTEGER")).arg(TBL_NAME).arg(FLD_TS_EDIT);
    if (!q.exec(addTsEdit)){
        return {false, q.lastError().text()};
    }
    const QString addDel =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 INTEGER DEFAULT 0"))
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

BoolVariantResult_t TableNote::doUpgrade(const SqlEngine_t &engine, const V3_t &fromVersion)
{
    QSqlQuery q(engine->db());
    const QString addUuid =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 VARCHAR(24) NOT NULL DEFAULT \"\""))
            .arg(TBL_NAME).arg(FLD_UUID);
    if (!q.exec(addUuid)){
        return {false, q.lastError().text()};
    }

    // Before unique index we should create unique values
    BoolVariantResult_t created = createUuids(engine);
    if(!created.result()) {
        return created;
    }

    const QString addUuidIndex =
            QString(QStringLiteral("CREATE UNIQUE INDEX IF NOT EXISTS id_UNIQUE ON %1 (%2)"))
            .arg(TBL_NAME).arg(FLD_UUID);
    if (!q.exec(addUuidIndex)){
        return {false, q.lastError().text()};
    }

    return {true, fromVersion.value};
}

BoolVariantResult_t TableNote::createUuids(SqlEngineSharedPtr_t enginePtr)
{
    const QString empties =
            QString(QStringLiteral("SELECT %1 FROM %2 WHERE %3=\"\""))
            .arg(FLD_ID).arg(TBL_NAME).arg(FLD_UUID);
    QSqlQuery q(enginePtr->db());
    if (!q.exec(empties)){
        return {false, q.lastError().text()};
    }
    QList<qint64> recs;
    while(q.next()) {
        recs.append(q.value(0).toInt());
    }
    {
        const QString QRY_UPD(QStringLiteral("UPDATE %1 SET %2=:uuid WHERE %3=:id")
                              .arg(TBL_NAME).arg(FLD_UUID).arg(FLD_ID));
        QSqlQuery qUpd(enginePtr->db());
        if (!qUpd.prepare(QRY_UPD)){
            return {false, q.lastError().text()};
        }else{
            for(int i=0; i<recs.count(); ++i) {
                QString uuid = QUuid::createUuid().toRfc4122().toBase64();
                qUpd.bindValue(":uuid", uuid);
                const qint32 id = recs.at(i);
                qUpd.bindValue(":id", id);
                if (!qUpd.exec()){
                    return {false, qUpd.lastError().text()};
                }
            }
        }
    }

    return {true, QVariant()};
}

//======================================================================================================================

const QString TableAuthor::TBL_NAME         = QStringLiteral("tbl_authors");

const QString TableAuthor::FLD_ID           = QStringLiteral("id");
const QString TableAuthor::FLD_UUID         = QStringLiteral("uuid");
const QString TableAuthor::FLD_TITLE        = QStringLiteral("title");
const QString TableAuthor::FLD_TS_CREATE    = QStringLiteral("ts_create");
const QString TableAuthor::FLD_TS_EDIT      = QStringLiteral("ts_edit");
const QString TableAuthor::FLD_DEL          = QStringLiteral("deleted");

BoolVariantResult_t TableAuthor::doCreate(const SqlEngine_t &engine)
{
    const QString QRY_CREATE =
            QString(QStringLiteral("CREATE TABLE %1(%2 INTEGER PRIMARY KEY ASC, %3 VARCHAR, %4 INTEGER, "
                                   "%5 INTEGER, %6 INTEGER)"))
            .arg(TBL_NAME).arg(FLD_ID).arg(FLD_TITLE).arg(FLD_TS_CREATE).arg(FLD_TS_EDIT).arg(FLD_DEL);

    QSqlQuery q(engine->db());
    if (!q.exec(QRY_CREATE)){
        return {false, q.lastError().text()};
    }

    // 'System author': default value for "orphan" notes.
    const QString QRY_INSERT(QStringLiteral("INSERT INTO %1(%2, %3, %4, %5) "
                                            "VALUES(:title, :ts_create, :ts_edit, :del)")
                             .arg(TBL_NAME).arg(FLD_TITLE).arg(FLD_TS_CREATE).arg(FLD_TS_EDIT).arg(FLD_DEL));
    if (!q.prepare(QRY_INSERT)){
        return {false, q.lastError().text()};
    }
    const qint64 ts = QDateTime::currentMSecsSinceEpoch();
    q.bindValue(":title", model::Constants::DEFAULT_AUTHOR);
    q.bindValue(":ts_create", ts);
    q.bindValue(":ts_edit", ts);
    q.bindValue(":del", 0);
    if (!q.exec()){
        return {false, q.lastError().text()};
    }
    return {true, q.lastInsertId().toString()};
}

BoolVariantResult_t TableAuthor::doUpgrade(const SqlEngine_t &engine, const V1_t &fromVersion)
{
    Q_UNUSED(fromVersion);
    return doCreate(engine);
}

BoolVariantResult_t TableAuthor::doUpgrade(const SqlEngine_t &engine, const V2_t &fromVersion)
{
    // 'Author' table and data
    QSqlQuery q(engine->db());

    const QString addUuid =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 VARCHAR(24) NOT NULL DEFAULT \"\""))
            .arg(TBL_NAME).arg(FLD_UUID);
    if (!q.exec(addUuid)){
        return {false, q.lastError().text()};
    }

    // Before unique index we should create unique values
    BoolVariantResult_t created = createUuids(engine);
    if(!created.result()) {
        return created;
    }

    //
    const QString addUuidIndex =
            QString(QStringLiteral("CREATE UNIQUE INDEX IF NOT EXISTS id_UNIQUE ON %1 (%2)"))
            .arg(TBL_NAME).arg(FLD_UUID);
    if (!q.exec(addUuidIndex)){
        return {false, q.lastError().text()};
    }

    const QString addTsCreate =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 INTEGER")).arg(TBL_NAME).arg(FLD_TS_CREATE);
    if (!q.exec(addTsCreate)){
        return {false, q.lastError().text()};
    }
    const QString addTsEdit =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 INTEGER")).arg(TBL_NAME).arg(FLD_TS_EDIT);
    if (!q.exec(addTsEdit)){
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

BoolVariantResult_t TableAuthor::doUpgrade(const SqlEngine_t &engine, const V3_t &fromVersion)
{
    QSqlQuery q(engine->db());
    const QString addUuid =
            QString(QStringLiteral("ALTER TABLE %1 ADD COLUMN %2 VARCHAR(24) NOT NULL DEFAULT \"\""))
            .arg(TBL_NAME).arg(FLD_UUID);
    if (!q.exec(addUuid)){
        return {false, q.lastError().text()};
    }

    // Before unique index we should create unique values
    BoolVariantResult_t created = createUuids(engine);
    if(!created.result()) {
        return created;
    }

    const QString addUuidIndex =
            QString(QStringLiteral("CREATE UNIQUE INDEX IF NOT EXISTS id_UNIQUE ON %1 (%2)"))
            .arg(TBL_NAME).arg(FLD_UUID);
    if (!q.exec(addUuidIndex)){
        return {false, q.lastError().text()};
    }

    return {true, fromVersion.value};
}

BoolVariantResult_t TableAuthor::createUuids(SqlEngineSharedPtr_t enginePtr)
{
    const QString empties =
            QString(QStringLiteral("SELECT %1 FROM %2 WHERE %3=\"\""))
            .arg(FLD_ID).arg(TBL_NAME).arg(FLD_UUID);
    QSqlQuery q(enginePtr->db());
    if (!q.exec(empties)){
        return {false, q.lastError().text()};
    }
    QList<qint64> recs;
    while(q.next()) {
        recs.append(q.value(0).toInt());
    }
    {
        const QString QRY_UPD(QStringLiteral("UPDATE %1 SET %2=:uuid WHERE %3=:id")
                              .arg(TBL_NAME).arg(FLD_UUID).arg(FLD_ID));
        QSqlQuery qUpd(enginePtr->db());
        if (!qUpd.prepare(QRY_UPD)){
            return {false, q.lastError().text()};
        }else{
            for(int i=0; i<recs.count(); ++i) {
                QString uuid = QUuid::createUuid().toRfc4122().toBase64();
                qUpd.bindValue(":uuid", uuid);
                const qint32 id = recs.at(i);
                qUpd.bindValue(":id", id);
                if (!qUpd.exec()){
                    return {false, qUpd.lastError().text()};
                }
            }
        }
    }

    return {true, QVariant()};
}

//======================================================================================================================

const QString TableTags::TBL_NAME       = QStringLiteral("tbl_tags");

const QString TableTags::FLD_ID         = QStringLiteral("id");
const QString TableTags::FLD_TEXT       = QStringLiteral("title");
const QString TableTags::FLD_DEL        = QStringLiteral("deleted");

BoolVariantResult_t TableTags::doCreate(const SqlEngine_t &engine)
{
    const QString QRY_CREATE =
            QString(QStringLiteral("CREATE TABLE %1(%2 INTEGER PRIMARY KEY ASC, %3 VARCHAR, %4 INTEGER)"))
            .arg(TBL_NAME).arg(FLD_ID).arg(FLD_TEXT).arg(FLD_DEL);

    QSqlQuery q(engine->db());
    if (!q.exec(QRY_CREATE)){
        return {false, q.lastError().text()};
    }

    // 'Default tag': default value for notes without tags.
    const QString QRY_INSERT(QStringLiteral("INSERT INTO %1(%2, %3) VALUES(:text, :del)")
                             .arg(TBL_NAME).arg(FLD_TEXT).arg(FLD_DEL));
    if (!q.prepare(QRY_INSERT)){
        return {false, q.lastError().text()};
    }
    q.bindValue(":text", model::Constants::TAG_INCOMING);
    q.bindValue(":del", 0);
    if (!q.exec()){
        return {false, q.lastError().text()};
    }
    return {true, q.lastInsertId().toString()};
}

BoolVariantResult_t TableTags::doUpgrade(const SqlEngine_t &engine, const V1_t &fromVersion)
{
    Q_UNUSED(fromVersion);
    return doCreate(engine);
}

BoolVariantResult_t TableTags::doUpgrade(const SqlEngine_t &engine, const V2_t &fromVersion)
{
    Q_UNUSED(fromVersion);
    return doCreate(engine);
}

BoolVariantResult_t TableTags::doUpgrade(const SqlEngine_t &engine, const V3_t &fromVersion)
{
    Q_UNUSED(engine);
    Q_UNUSED(fromVersion);
    return {true, fromVersion.value};
}

//======================================================================================================================

const QString TableTagsNotes::TBL_NAME      = QStringLiteral("tbl_tags_notes");

const QString TableTagsNotes::FLD_ID        = QStringLiteral("id");
const QString TableTagsNotes::FLD_FK_TAGS   = QStringLiteral("fk_tags");
const QString TableTagsNotes::FLD_FK_NOTES  = QStringLiteral("fk_notes");

BoolVariantResult_t TableTagsNotes::doCreate(const SqlEngine_t &engine)
{
    const QString QRY_CREATE =
            QString(QStringLiteral("CREATE TABLE %1(%2 INTEGER PRIMARY KEY, %3 VARCHAR, %4 INTEGER)"))
            .arg(TBL_NAME).arg(FLD_ID).arg(FLD_FK_TAGS).arg(FLD_FK_NOTES);

    QSqlQuery q(engine->db());
    if (!q.exec(QRY_CREATE)){
        return {false, q.lastError().text()};
    }

    return {true, QVariant()};
}

BoolVariantResult_t TableTagsNotes::doUpgrade(const SqlEngine_t &engine, const V1_t &fromVersion)
{
    Q_UNUSED(fromVersion);
    return doCreate(engine);
}

BoolVariantResult_t TableTagsNotes::doUpgrade(const SqlEngine_t &engine, const V2_t &fromVersion)
{
    Q_UNUSED(fromVersion);
    return doCreate(engine);
}

BoolVariantResult_t TableTagsNotes::doUpgrade(const SqlEngine_t &engine, const V3_t &fromVersion)
{
    Q_UNUSED(engine);
    return {true, fromVersion.value};
}

//======================================================================================================================

} // namespace sqlite

} // namespace storage
