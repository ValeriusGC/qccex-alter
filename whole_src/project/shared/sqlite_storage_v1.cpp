#include "sqlite_storage_v1.h"

using namespace storage::sqlite::v1;

const qint32 VERSION = 1;

const QString TableCfg::TBL_NAME        = QStringLiteral("tbl_config");
const QString TableCfg::FLD_ID          = QStringLiteral("id");
const QString TableCfg::FLD_THE_KEY     = QStringLiteral("the_key");
const QString TableCfg::FLD_THE_VALUE   = QStringLiteral("the_value");

const QString TableCfg::QRY_CREATE      =
        QString(QStringLiteral("create table %1(%2 integer primary key, %3 varchar, %4 varchar)"))
        .arg(TableCfg::TBL_NAME).arg(TableCfg::FLD_ID).arg(TableCfg::FLD_THE_KEY).arg(TableCfg::FLD_THE_VALUE);


const QString TableNote::TBL_NAME       = QStringLiteral("tbl_notes");
const QString TableNote::FLD_ID         = QStringLiteral("id");
const QString TableNote::FLD_THE_TEXT   = QStringLiteral("the_text");

const QString TableNote::QRY_CREATE      =
        QString(QStringLiteral("create table %1(%2 integer primary key, %3 varchar)"))
        .arg(TableNote::TBL_NAME).arg(TableNote::FLD_ID).arg(TableNote::FLD_THE_TEXT);
