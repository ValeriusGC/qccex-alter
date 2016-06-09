#include <QSqlQuery>

#include "base_table.h"
#include "storage_globals.h"

using namespace storage;

BaseTable::BaseTable(QObject *parent) : QObject(parent)
{
}

BaseTable::~BaseTable()
{
}

SqlStringResult_t BaseTable::create(const QSqlDatabase &db)
{
    SqlStringResult_t res = {QSqlError(), ""};
    switch (STORAGE_CURR_VER) {
    case 1:
        res = createV1(db);
        break;
    case 2:
//        res = createV2();
//        break;
    default:
        break;
    }
    return res;
}

QVector<QString> &BaseTable::fieldNames()
{
    return m_fieldNames;
}

bool BaseTable::contains(const QString &column, const QVariant &value)
{
    Q_UNUSED(column);
    Q_UNUSED(value);
//    QSqlDatabase db = QSqlDatabase::database();
//    if(!db.isOpen()) {
//        return false;
//    }

//    QSqlQuery q(db);
//    q.prepare(QString("SELECT COUNT (*) FROM %1 WHERE %2 = '%3'").arg(name()).arg(column).arg(value));
//    bool isExec = q.exec();
//    if(!isExec) {
//        return false;
//    }

//    if (q.next()) {
//        return true;
//    }else{
//        return false;
//    }
    return false;

}

